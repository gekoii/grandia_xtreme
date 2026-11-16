/*
 * The port's EE kernel: the kernel calls the game makes (threads,
 * semaphores, interrupt and DMA handlers, alarms, cache maintenance) as a
 * cooperative scheduler, plan "Kernel" in docs/web_port.md.
 *
 * The EE kernel's scheduling is priority-based and never time-sliced: a
 * thread runs until it blocks (WaitSema on a zero count, SleepThread,
 * SuspendThread on itself), yields (RotateThreadReadyQueue), exits, or a
 * kernel call or interrupt makes a higher-priority thread ready. That maps
 * onto cooperative contexts exactly, apart from when interrupts arrive:
 * here they come only when the platform raises them (grx_kernel.h), from
 * its frame loop or from the idle hook.
 *
 * Model (as the EE kernel's, ITRON-like):
 * - Priorities 0 (highest) to 127. One FIFO ready queue per priority; the
 *   running thread stays in its queue, normally at the head, so the thread
 *   that runs is always the head of the highest non-empty queue. A thread
 *   made ready joins the tail of its queue: it preempts the running thread
 *   only if its priority is strictly higher. RotateThreadReadyQueue moves a
 *   queue's head to its tail; ChangeThreadPriority moves the thread to the
 *   tail of its new priority's queue.
 * - Thread 0 is the idle loop (GetThreadId returns 0 in handlers that
 *   interrupted it), thread 1 is the main thread (the OS thread that first
 *   used the kernel), created threads are 2 to 255. Ids of 0 for "self".
 * - Calls from interrupt handlers (the i* calls, and any other) only make
 *   threads ready; the switch happens when the handler chain ends.
 * - DIntr/EIntr are per thread (the kernel keeps Status per thread); the
 *   idle loop runs with interrupts on. Interrupts raised while they are off,
 *   or while the cause is masked, are latched and delivered when possible.
 * - Errors return -1 (the EE kernel's calls return negative values; the
 *   game only tests for < 0).
 *
 * This file must not include <windows.h> (its CreateThread, ExitThread,
 * SuspendThread... clash with the game's); contexts are in fiber.c.
 */
#include <stdlib.h>
#include <string.h>
/* Before the declarations: the EE calls named like Windows', renamed on
   x86-64 */
#include "grx_ee_names.h"
#include "sdk/eekernel.h"
#include "grx_kernel.h"
#include "grx_fiber.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

/* The main thread's priority before the game changes it. SetupThread
   gives the main thread 0 (libkernel's InitThread, unused by this game,
   creates its helper at 0 and then lowers the main thread to 1). */
#define MAIN_PRIORITY 0

#define MAX_THREADS 256
#define MAX_SEMAS 256
#define MAX_HANDLERS 128
#define MAX_ALARMS 64
#define NUM_PRIO 128
#define NUM_CAUSES 16
#define FIBER_STACK (1024 * 1024)

/* ReferThreadStatus's status values */
#define THS_RUN 0x01
#define THS_READY 0x02
#define THS_WAIT 0x04
#define THS_SUSPEND 0x08
#define THS_WAITSUSPEND 0x0C
#define THS_DORMANT 0x10

/* ...and waitType */
#define TSW_NONE 0
#define TSW_SLEEP 1
#define TSW_SEMA 2

/* ---------------------------------------------------------------------- */
/* The kernel calls, with the EE kernel's signatures (the game's own
   declarations vary in return type, which cdecl doesn't mind) */

typedef s32 (*IntcHandler)(s32 cause, void *arg, void *addr);
typedef void (*AlarmHandler)(s32 id, u16 time, void *arg);

s32 CreateThread(ThreadParam *param);
s32 DeleteThread(s32 tid);
s32 StartThread(s32 tid, void *arg);
void ExitThread(void);
void ExitDeleteThread(void);
s32 TerminateThread(s32 tid);
s32 iTerminateThread(s32 tid);
s32 ChangeThreadPriority(s32 tid, s32 prio);
s32 iChangeThreadPriority(s32 tid, s32 prio);
s32 RotateThreadReadyQueue(s32 prio);
s32 _iRotateThreadReadyQueue(s32 prio);
s32 ReleaseWaitThread(s32 tid);
s32 iReleaseWaitThread(s32 tid);
s32 GetThreadId(void);
s32 ReferThreadStatus(s32 tid, ThreadParam *info);
s32 iReferThreadStatus(s32 tid, ThreadParam *info);
s32 SleepThread(void);
s32 WakeupThread(s32 tid);
s32 _iWakeupThread(s32 tid);
s32 CancelWakeupThread(s32 tid);
s32 iCancelWakeupThread(s32 tid);
s32 SuspendThread(s32 tid);
s32 _iSuspendThread(s32 tid);
s32 ResumeThread(s32 tid);
s32 iResumeThread(s32 tid);

s32 CreateSema(SemaParam *param);
s32 DeleteSema(s32 sid);
s32 SignalSema(s32 sid);
s32 iSignalSema(s32 sid);
s32 WaitSema(s32 sid);
s32 PollSema(s32 sid);
s32 iPollSema(s32 sid);
s32 ReferSemaStatus(s32 sid, SemaParam *info);
s32 iReferSemaStatus(s32 sid, SemaParam *info);

s32 AddIntcHandler(s32 cause, void *handler, s32 next);
s32 AddIntcHandler2(s32 cause, void *handler, s32 next, void *arg);
s32 RemoveIntcHandler(s32 cause, s32 hid);
s32 AddDmacHandler(s32 channel, void *handler, s32 next);
s32 AddDmacHandler2(s32 channel, void *handler, s32 next, void *arg);
s32 RemoveDmacHandler(s32 channel, s32 hid);
s32 EnableIntc(s32 cause);
s32 DisableIntc(s32 cause);
s32 EnableDmac(s32 channel);
s32 DisableDmac(s32 channel);
s32 _EnableIntc(s32 cause);
s32 _DisableIntc(s32 cause);
s32 _EnableDmac(s32 channel);
s32 _DisableDmac(s32 channel);
s32 _iEnableIntc(s32 cause);
s32 _iDisableIntc(s32 cause);
s32 _iEnableDmac(s32 channel);
s32 _iDisableDmac(s32 channel);
s32 DIntr(void);
s32 EIntr(void);

s32 SetAlarm(u16 time, void *handler, void *arg);
s32 iSetAlarm(u16 time, void *handler, void *arg);
s32 ReleaseAlarm(s32 id);
s32 iReleaseAlarm(s32 id);

void FlushCache(s32 op);
void iFlushCache(s32 op);
void SyncDCache(void *start, void *end);
void _SyncDCache(u32 start, u32 end);
void _InvalidDCache(u32 start, u32 end);
void Exit(s32 status);

/* ---------------------------------------------------------------------- */
/* State */

typedef struct KThread KThread;

typedef struct {
    KThread *head;
    KThread *tail;
} KQueue;

struct KThread {
    int used;
    int id;
    int dormant;
    int wait_type;   /* TSW_* */
    int wait_id;     /* the semaphore of TSW_SEMA */
    int wait_result; /* what WaitSema returns once released */
    int suspend;     /* SuspendThread nesting */
    int prio;
    int init_prio;
    int wakeup;      /* wakeup count */
    int irq;         /* DIntr/EIntr state while switched out */
    void *entry;
    void *stack;
    s32 stack_size;
    void *gp;
    u32 attr;
    u32 option;
    void *arg;
    KQueue *queue;   /* the ready or semaphore queue it is in */
    KThread *next;
    KThread *prev;
    GrxFiber *fiber;
};

typedef struct {
    int used;
    s32 count;
    s32 max;
    s32 init;
    u32 attr;
    u32 option;
    int nwait;
    KQueue waiters;
} KSema;

typedef struct {
    int used;
    int dmac;   /* 0: INTC cause, 1: DMAC channel */
    int cause;
    int seq;    /* order in the chain */
    void *fn;
    void *arg;
} KHandler;

typedef struct {
    int used;
    unsigned long long due;
    u16 time;
    void *fn;
    void *arg;
    unsigned int seq;
} KAlarm;

static int inited;
static KThread threads[MAX_THREADS];
static KThread idle_thread;
static KThread *cur;          /* the running thread, &idle_thread in the idle loop */
static GrxFiber *on_fiber;    /* the context actually running (the idle loop
                                 runs on the context of whoever went idle) */
static GrxFiber *main_fiber;
static KQueue ready[NUM_PRIO];

static KSema semas[MAX_SEMAS];
static KHandler handlers[MAX_HANDLERS];
static int handler_seq_head = 0;  /* decreasing: added at the head */
static int handler_seq_tail = 0;  /* increasing: added at the tail */
static KAlarm alarms[MAX_ALARMS];
static unsigned int alarm_seq;

static int irq_enabled = 1;   /* the running thread's DIntr/EIntr state */
static int in_interrupt;
static int idling;
static int hold;              /* > 0: grx_kernel_field() in progress */
static u32 intc_mask, intc_pending;
static u32 dmac_mask, dmac_pending;
static unsigned long long hsync_now;
static unsigned int field_count;
static void (*idle_hook)(void);

/* Where the main thread last called WaitSema or SleepThread from: with
   the stack's depth, which call a save state's stack stops at (state.c) */
static void *main_wait_site;

static GrxFiber *reap_list[MAX_THREADS];
static int reap_count;

static void schedule(void);

/* ---------------------------------------------------------------------- */
/* Queues */

static void q_append(KQueue *q, KThread *t) {
    t->queue = q;
    t->next = NULL;
    t->prev = q->tail;
    if (q->tail != NULL) {
        q->tail->next = t;
    } else {
        q->head = t;
    }
    q->tail = t;
}

static void q_remove(KThread *t) {
    KQueue *q = t->queue;
    if (q == NULL) {
        return;
    }
    if (t->prev != NULL) {
        t->prev->next = t->next;
    } else {
        q->head = t->next;
    }
    if (t->next != NULL) {
        t->next->prev = t->prev;
    } else {
        q->tail = t->prev;
    }
    t->queue = NULL;
    t->next = t->prev = NULL;
}

static int is_ready(const KThread *t) {
    return t->used && !t->dormant && t->wait_type == TSW_NONE && t->suspend == 0;
}

/* Put a thread whose state changed into its ready queue if it is now ready */
static void make_ready(KThread *t) {
    if (is_ready(t) && t->queue == NULL) {
        q_append(&ready[t->prio], t);
    }
}

static void make_unready(KThread *t) {
    if (t->queue != NULL && t->queue >= ready && t->queue < ready + NUM_PRIO) {
        q_remove(t);
    }
}

static KThread *pick(void) {
    int p;
    for (p = 0; p < NUM_PRIO; p++) {
        if (ready[p].head != NULL) {
            return ready[p].head;
        }
    }
    return NULL;
}

/* ---------------------------------------------------------------------- */
/* Init, contexts */

void grx_kernel_init(void) {
    KThread *m;

    if (inited) {
        return;
    }
    inited = 1;
    main_fiber = grx_fiber_main();
    on_fiber = main_fiber;

    idle_thread.id = 0;
    idle_thread.prio = NUM_PRIO;
    idle_thread.irq = 1;

    m = &threads[1];
    memset(m, 0, sizeof(*m));
    m->used = 1;
    m->id = 1;
    m->prio = m->init_prio = MAIN_PRIORITY;
    m->irq = 1;
    m->fiber = main_fiber;
    q_append(&ready[m->prio], m);
    cur = m;
    irq_enabled = 1;
}

#define KINIT() do { if (!inited) grx_kernel_init(); } while (0)

static void reap(void) {
    int i, n = 0;
    for (i = 0; i < reap_count; i++) {
        if (reap_list[i] == on_fiber) {
            reap_list[n++] = reap_list[i];
        } else {
            grx_fiber_delete(reap_list[i]);
        }
    }
    reap_count = n;
}

/* Free a context once nothing runs on it */
static void retire_fiber(GrxFiber *f) {
    if (f == NULL || f == main_fiber) {
        return;
    }
    if (f == on_fiber) {
        if (reap_count >= MAX_THREADS) {
            grx_fatal("kernel: too many contexts waiting to be freed");
        }
        reap_list[reap_count++] = f;
    } else {
        grx_fiber_delete(f);
    }
}

static void set_current(KThread *t) {
    cur->irq = irq_enabled;
    cur = t;
    irq_enabled = t->irq;
}

static void switch_to(KThread *t) {
    set_current(t);
    if (t->fiber != on_fiber) {
        on_fiber = t->fiber;
        grx_fiber_switch(t->fiber);
        /* Resumed: whoever switched here set cur and on_fiber */
        reap();
    }
}

static void exit_current(int delete_it);

static void thread_main(void *p) {
    KThread *t = p;
    void (*entry)(void *) = (void (*)(void *))t->entry;

    reap();
    schedule(); /* interrupts latched before it ran, if any */
    entry(t->arg);
    /* Returning from the entry is ExitThread (its return address is) */
    exit_current(0);
}

/* ---------------------------------------------------------------------- */
/* Interrupt delivery */

static KAlarm *next_alarm(void) {
    KAlarm *best = NULL;
    int i;
    for (i = 0; i < MAX_ALARMS; i++) {
        KAlarm *a = &alarms[i];
        if (a->used && (best == NULL || a->due < best->due || (a->due == best->due && a->seq < best->seq))) {
            best = a;
        }
    }
    return best;
}

static int deliverable(void) {
    KAlarm *a;
    if (in_interrupt || !irq_enabled) {
        return 0;
    }
    if ((intc_pending & intc_mask) || (dmac_pending & dmac_mask)) {
        return 1;
    }
    a = next_alarm();
    return a != NULL && a->due <= hsync_now;
}

static void run_chain(int dmac, int cause) {
    int ids[MAX_HANDLERS];
    int n = 0, i, j;

    /* Snapshot the chain in order: handlers may remove themselves */
    for (i = 0; i < MAX_HANDLERS; i++) {
        if (handlers[i].used && handlers[i].dmac == dmac && handlers[i].cause == cause) {
            for (j = n; j > 0 && handlers[ids[j - 1]].seq > handlers[i].seq; j--) {
                ids[j] = ids[j - 1];
            }
            ids[j] = i;
            n++;
        }
    }
    in_interrupt = 1;
    for (i = 0; i < n; i++) {
        KHandler *h = &handlers[ids[i]];
        if (!h->used) {
            continue;
        }
        /* A negative return skips the rest of the chain */
        if (((IntcHandler)h->fn)(cause, h->arg, NULL) < 0) {
            break;
        }
    }
    in_interrupt = 0;
}

static void fire_alarm(KAlarm *a) {
    AlarmHandler fn = (AlarmHandler)a->fn;
    s32 id = (s32)(a - alarms) + 1;
    void *arg = a->arg;
    u16 time = a->time;

    a->used = 0;
    in_interrupt = 1;
    fn(id, time, arg);
    in_interrupt = 0;
}

static int lowest_bit(u32 v) {
    int i;
    for (i = 0; i < 32; i++) {
        if (v & (1u << i)) {
            return i;
        }
    }
    return -1;
}

/* Run what is pending and deliverable now: INTC causes, DMAC channels,
   then alarms that are due */
static void deliver(void) {
    while (!in_interrupt && irq_enabled) {
        u32 ip = intc_pending & intc_mask;
        u32 dp = dmac_pending & dmac_mask;
        KAlarm *a;
        if (ip) {
            int c = lowest_bit(ip);
            intc_pending &= ~(1u << c);
            run_chain(0, c);
            continue;
        }
        if (dp) {
            int c = lowest_bit(dp);
            dmac_pending &= ~(1u << c);
            run_chain(1, c);
            continue;
        }
        a = next_alarm();
        if (a != NULL && a->due <= hsync_now) {
            fire_alarm(a);
            continue;
        }
        break;
    }
}

/* After something changed what is ready: switch if a thread of higher
   priority than the running one is ready, unless that has to wait for the
   end of an interrupt, the idle loop or the field */
static void maybe_schedule(void) {
    if (!in_interrupt && !idling && hold == 0) {
        schedule();
    }
}

static int vblank_handler_enabled(void) {
    int i;
    u32 vb = (1u << GRX_INTC_VBLANK_START) | (1u << GRX_INTC_VBLANK_END);
    for (i = 0; i < MAX_HANDLERS; i++) {
        if (handlers[i].used && !handlers[i].dmac && ((1u << handlers[i].cause) & vb & intc_mask)) {
            return 1;
        }
    }
    return 0;
}

static void default_idle(void) {
    KAlarm *a = next_alarm();
    if (a != NULL) {
        grx_kernel_hsync(a->due > hsync_now ? (int)(a->due - hsync_now) : 0);
        return;
    }
    if (vblank_handler_enabled()) {
        grx_kernel_field();
        return;
    }
    grx_kernel_dump();
    grx_fatal("kernel: deadlock, every thread waits and no interrupt can come");
}

/* Every thread waits: run the idle loop (thread 0, interrupts on) once */
static void idle_once(void) {
    set_current(&idle_thread);
    irq_enabled = 1;
    idling = 1;
    deliver();
    if (pick() == NULL) {
        if (idle_hook != NULL) {
            idle_hook();
        } else {
            default_idle();
        }
    }
    idling = 0;
}

/* Run the highest-priority ready thread (and pending interrupts first) */
static void schedule(void) {
    if (in_interrupt || idling) {
        return;
    }
    for (;;) {
        KThread *t;
        if (deliverable()) {
            deliver();
            continue;
        }
        t = pick();
        if (t == NULL) {
            idle_once();
            continue;
        }
        if (t == cur) {
            return;
        }
        switch_to(t);
    }
}

/* ---------------------------------------------------------------------- */
/* Threads */

static KThread *get_thread(s32 tid) {
    if (tid == 0) {
        return cur != &idle_thread ? cur : NULL;
    }
    if (tid < 0 || tid >= MAX_THREADS || !threads[tid].used) {
        return NULL;
    }
    return &threads[tid];
}

s32 CreateThread(ThreadParam *param) {
    int i;
    KThread *t;

    KINIT();
    if (param == NULL || param->entry == NULL || param->initPriority < 0 || param->initPriority >= NUM_PRIO) {
        return -1;
    }
    for (i = 2; i < MAX_THREADS; i++) {
        if (!threads[i].used) {
            break;
        }
    }
    if (i == MAX_THREADS) {
        return -1;
    }
    t = &threads[i];
    memset(t, 0, sizeof(*t));
    t->used = 1;
    t->id = i;
    t->dormant = 1;
    t->entry = param->entry;
    t->stack = param->stack;
    t->stack_size = param->stackSize;
    t->gp = param->gpReg;
    t->init_prio = t->prio = param->initPriority;
    t->attr = param->attr;
    t->option = param->option;
    t->irq = 1;
    return i;
}

s32 StartThread(s32 tid, void *arg) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || !t->dormant || t->entry == NULL) {
        return -1;
    }
    retire_fiber(t->fiber);
    t->fiber = grx_fiber_create(FIBER_STACK, thread_main, t);
    t->arg = arg;
    t->prio = t->init_prio;
    t->wakeup = 0;
    t->suspend = 0;
    t->wait_type = TSW_NONE;
    t->irq = 1;
    t->dormant = 0;
    make_ready(t);
    maybe_schedule();
    return t->id;
}

/* Leave a semaphore's wait queue, if in one */
static void unwait(KThread *t, int result) {
    if (t->wait_type == TSW_SEMA) {
        q_remove(t);
        semas[t->wait_id].nwait--;
    }
    t->wait_type = TSW_NONE;
    t->wait_id = 0;
    t->wait_result = result;
}

static void make_dormant(KThread *t) {
    make_unready(t);
    unwait(t, -1);
    t->dormant = 1;
    t->suspend = 0;
    t->wakeup = 0;
}

static void exit_current(int delete_it) {
    KThread *t = cur;

    if (in_interrupt || t == &idle_thread) {
        grx_log("kernel: ExitThread outside a thread");
        return;
    }
    make_dormant(t);
    if (delete_it) {
        t->used = 0;
        retire_fiber(t->fiber); /* deferred: it's running */
        t->fiber = NULL;
    }
    schedule();
    grx_fatal("kernel: thread %d ran after exiting", t->id);
}

void ExitThread(void) {
    KINIT();
    exit_current(0);
}

void ExitDeleteThread(void) {
    KINIT();
    exit_current(1);
}

static s32 terminate(s32 tid, int resched) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t == cur || t->dormant) {
        return -1;
    }
    make_dormant(t);
    retire_fiber(t->fiber);
    t->fiber = NULL;
    if (resched) {
        maybe_schedule();
    }
    return t->id;
}

s32 TerminateThread(s32 tid) {
    return terminate(tid, 1);
}

s32 iTerminateThread(s32 tid) {
    return terminate(tid, 0);
}

s32 DeleteThread(s32 tid) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t == cur || !t->dormant || t->id == 1) {
        return -1;
    }
    retire_fiber(t->fiber);
    t->fiber = NULL;
    t->used = 0;
    return tid;
}

static s32 change_priority(s32 tid, s32 prio, int resched) {
    KThread *t;
    int old;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t->dormant || prio < 0 || prio >= NUM_PRIO) {
        return -1;
    }
    old = t->prio;
    t->prio = prio;
    if (t->queue != NULL && t->wait_type == TSW_NONE) {
        /* Ready (or running): to the tail of the new priority's queue */
        q_remove(t);
        q_append(&ready[prio], t);
    }
    if (resched) {
        maybe_schedule();
    }
    return old;
}

s32 ChangeThreadPriority(s32 tid, s32 prio) {
    return change_priority(tid, prio, 1);
}

s32 iChangeThreadPriority(s32 tid, s32 prio) {
    return change_priority(tid, prio, 0);
}

static s32 rotate(s32 prio, int resched) {
    KQueue *q;

    KINIT();
    if (prio < 0 || prio >= NUM_PRIO) {
        return -1;
    }
    q = &ready[prio];
    if (q->head != NULL && q->head != q->tail) {
        KThread *t = q->head;
        q_remove(t);
        q_append(q, t);
    }
    if (resched) {
        maybe_schedule();
    }
    return prio;
}

s32 RotateThreadReadyQueue(s32 prio) {
    return rotate(prio, 1);
}

s32 _iRotateThreadReadyQueue(s32 prio) {
    return rotate(prio, 0);
}

static s32 release_wait(s32 tid, int resched) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t->wait_type == TSW_NONE) {
        return -1;
    }
    unwait(t, -1);
    make_ready(t);
    if (resched) {
        maybe_schedule();
    }
    return t->id;
}

s32 ReleaseWaitThread(s32 tid) {
    return release_wait(tid, 1);
}

s32 iReleaseWaitThread(s32 tid) {
    return release_wait(tid, 0);
}

s32 GetThreadId(void) {
    KINIT();
    return cur->id;
}

static int thread_status(const KThread *t) {
    if (t->dormant) {
        return THS_DORMANT;
    }
    if (t == cur) {
        return THS_RUN;
    }
    if (t->wait_type != TSW_NONE) {
        return t->suspend ? THS_WAITSUSPEND : THS_WAIT;
    }
    return t->suspend ? THS_SUSPEND : THS_READY;
}

s32 ReferThreadStatus(s32 tid, ThreadParam *info) {
    KThread *t;
    int status;

    KINIT();
    t = get_thread(tid);
    if (t == NULL) {
        return -1;
    }
    status = thread_status(t);
    if (info != NULL) {
        info->status = status;
        info->entry = t->entry;
        info->stack = t->stack;
        info->stackSize = t->stack_size;
        info->gpReg = t->gp;
        info->initPriority = t->init_prio;
        info->currentPriority = t->prio;
        info->attr = t->attr;
        info->option = t->option;
        info->waitType = t->wait_type;
        info->waitId = t->wait_id;
        info->wakeupCount = t->wakeup;
    }
    return status;
}

s32 iReferThreadStatus(s32 tid, ThreadParam *info) {
    return ReferThreadStatus(tid, info);
}

s32 SleepThread(void) {
    KThread *t;

    KINIT();
    if (cur == &threads[1]) {
        main_wait_site = __builtin_return_address(0);
    }
    t = cur;
    if (in_interrupt || t == &idle_thread) {
        return -1;
    }
    if (t->wakeup > 0) {
        t->wakeup--;
        return t->id;
    }
    make_unready(t);
    t->wait_type = TSW_SLEEP;
    schedule();
    return t->id;
}

static s32 wakeup(s32 tid, int resched) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t->dormant) {
        return -1;
    }
    if (t->wait_type == TSW_SLEEP) {
        t->wait_type = TSW_NONE;
        make_ready(t);
        if (resched) {
            maybe_schedule();
        }
    } else {
        t->wakeup++;
    }
    return t->id;
}

s32 WakeupThread(s32 tid) {
    return wakeup(tid, 1);
}

s32 _iWakeupThread(s32 tid) {
    return wakeup(tid, 0);
}

s32 CancelWakeupThread(s32 tid) {
    KThread *t;
    int n;

    KINIT();
    t = get_thread(tid);
    if (t == NULL) {
        return -1;
    }
    n = t->wakeup;
    t->wakeup = 0;
    return n;
}

s32 iCancelWakeupThread(s32 tid) {
    return CancelWakeupThread(tid);
}

static s32 suspend(s32 tid, int resched) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t->dormant) {
        return -1;
    }
    t->suspend++;
    make_unready(t);
    if (resched) {
        maybe_schedule();
    }
    return t->id;
}

s32 SuspendThread(s32 tid) {
    return suspend(tid, 1);
}

s32 _iSuspendThread(s32 tid) {
    return suspend(tid, 0);
}

static s32 resume(s32 tid, int resched) {
    KThread *t;

    KINIT();
    t = get_thread(tid);
    if (t == NULL || t->suspend == 0) {
        return -1;
    }
    t->suspend--;
    make_ready(t);
    if (resched) {
        maybe_schedule();
    }
    return t->id;
}

s32 ResumeThread(s32 tid) {
    return resume(tid, 1);
}

s32 iResumeThread(s32 tid) {
    return resume(tid, 0);
}

/* ---------------------------------------------------------------------- */
/* Semaphores */

static KSema *get_sema(s32 sid) {
    if (sid <= 0 || sid >= MAX_SEMAS || !semas[sid].used) {
        return NULL;
    }
    return &semas[sid];
}

s32 CreateSema(SemaParam *param) {
    int i;
    KSema *s;

    KINIT();
    if (param == NULL || param->initCount < 0) {
        return -1;
    }
    /* Ids from 1: some of the game's code takes 0 for "none" */
    for (i = 1; i < MAX_SEMAS; i++) {
        if (!semas[i].used) {
            break;
        }
    }
    if (i == MAX_SEMAS) {
        return -1;
    }
    s = &semas[i];
    memset(s, 0, sizeof(*s));
    s->used = 1;
    s->count = param->initCount;
    s->init = param->initCount;
    s->max = param->maxCount;
    s->attr = param->attr;
    s->option = param->option;
    return i;
}

s32 DeleteSema(s32 sid) {
    KSema *s;

    KINIT();
    s = get_sema(sid);
    if (s == NULL) {
        return -1;
    }
    /* Waiters are released with an error */
    while (s->waiters.head != NULL) {
        KThread *t = s->waiters.head;
        unwait(t, -1);
        make_ready(t);
    }
    s->used = 0;
    maybe_schedule();
    return sid;
}

static s32 sema_signal(s32 sid, int resched) {
    KSema *s;

    KINIT();
    s = get_sema(sid);
    if (s == NULL) {
        return -1;
    }
    if (s->waiters.head != NULL) {
        /* The first waiter takes it (FIFO); the count stays 0 */
        KThread *t = s->waiters.head;
        unwait(t, sid);
        make_ready(t);
        if (resched) {
            maybe_schedule();
        }
    } else {
        if (s->max > 0 && s->count >= s->max) {
            return -1; /* KE_SEMA_OVF */
        }
        s->count++;
    }
    return sid;
}

s32 SignalSema(s32 sid) {
    return sema_signal(sid, 1);
}

s32 iSignalSema(s32 sid) {
    return sema_signal(sid, 0);
}

s32 WaitSema(s32 sid) {
    KSema *s;
    KThread *t;

    KINIT();
    if (cur == &threads[1]) {
        main_wait_site = __builtin_return_address(0);
    }
    s = get_sema(sid);
    if (s == NULL) {
        return -1;
    }
    if (s->count > 0) {
        s->count--;
        return sid;
    }
    t = cur;
    if (in_interrupt || t == &idle_thread) {
        grx_log("kernel: WaitSema(%d) would block in an interrupt handler", sid);
        return -1;
    }
    make_unready(t);
    t->wait_type = TSW_SEMA;
    t->wait_id = sid;
    t->wait_result = -1;
    q_append(&s->waiters, t);
    s->nwait++;
    schedule();
    return t->wait_result;
}

s32 PollSema(s32 sid) {
    KSema *s;

    KINIT();
    s = get_sema(sid);
    if (s == NULL || s->count <= 0) {
        return -1; /* KE_SEMA_ZERO */
    }
    s->count--;
    return sid;
}

s32 iPollSema(s32 sid) {
    return PollSema(sid);
}

s32 ReferSemaStatus(s32 sid, SemaParam *info) {
    KSema *s;

    KINIT();
    s = get_sema(sid);
    if (s == NULL) {
        return -1;
    }
    if (info != NULL) {
        info->currentCount = s->count;
        info->maxCount = s->max;
        info->initCount = s->init;
        info->numWaitThreads = s->nwait;
        info->attr = s->attr;
        info->option = s->option;
    }
    return sid;
}

s32 iReferSemaStatus(s32 sid, SemaParam *info) {
    return ReferSemaStatus(sid, info);
}

/* ---------------------------------------------------------------------- */
/* Interrupt and DMA handlers */

static s32 add_handler(int dmac, s32 cause, void *fn, s32 next, void *arg) {
    int i;

    KINIT();
    if (cause < 0 || cause >= NUM_CAUSES || fn == NULL) {
        return -1;
    }
    for (i = 0; i < MAX_HANDLERS; i++) {
        if (!handlers[i].used) {
            break;
        }
    }
    if (i == MAX_HANDLERS) {
        return -1;
    }
    handlers[i].used = 1;
    handlers[i].dmac = dmac;
    handlers[i].cause = cause;
    handlers[i].fn = fn;
    handlers[i].arg = arg;
    /* next 0: the head of the chain; otherwise (-1) the tail */
    handlers[i].seq = next == 0 ? --handler_seq_head : ++handler_seq_tail;
    return i + 1; /* ids from 1: the game takes 0 for "none" */
}

static s32 remove_handler(int dmac, s32 cause, s32 hid) {
    int i, n = 0;
    KHandler *h;

    KINIT();
    if (hid <= 0 || hid > MAX_HANDLERS) {
        return -1;
    }
    h = &handlers[hid - 1];
    if (!h->used || h->dmac != dmac || h->cause != cause) {
        return -1;
    }
    h->used = 0;
    for (i = 0; i < MAX_HANDLERS; i++) {
        n += handlers[i].used && handlers[i].dmac == dmac && handlers[i].cause == cause;
    }
    return n; /* the handlers left on the cause */
}

s32 AddIntcHandler(s32 cause, void *handler, s32 next) {
    return add_handler(0, cause, handler, next, NULL);
}

s32 AddIntcHandler2(s32 cause, void *handler, s32 next, void *arg) {
    return add_handler(0, cause, handler, next, arg);
}

s32 RemoveIntcHandler(s32 cause, s32 hid) {
    return remove_handler(0, cause, hid);
}

s32 AddDmacHandler(s32 channel, void *handler, s32 next) {
    return add_handler(1, channel, handler, next, NULL);
}

s32 AddDmacHandler2(s32 channel, void *handler, s32 next, void *arg) {
    return add_handler(1, channel, handler, next, arg);
}

s32 RemoveDmacHandler(s32 channel, s32 hid) {
    return remove_handler(1, channel, hid);
}

/* Returns 1 if the mask bit changed, 0 if it was already so */
static s32 set_mask(u32 *mask, s32 cause, int on) {
    u32 bit;
    u32 old;

    KINIT();
    if (cause < 0 || cause >= NUM_CAUSES) {
        return -1;
    }
    bit = 1u << cause;
    old = *mask & bit;
    if (on) {
        *mask |= bit;
    } else {
        *mask &= ~bit;
    }
    return on ? old == 0 : old != 0;
}

s32 _iEnableIntc(s32 cause) {
    return set_mask(&intc_mask, cause, 1);
}

s32 _iDisableIntc(s32 cause) {
    return set_mask(&intc_mask, cause, 0);
}

s32 _iEnableDmac(s32 channel) {
    return set_mask(&dmac_mask, channel, 1);
}

s32 _iDisableDmac(s32 channel) {
    return set_mask(&dmac_mask, channel, 0);
}

s32 _EnableIntc(s32 cause) {
    s32 r = _iEnableIntc(cause);
    deliver();
    maybe_schedule();
    return r;
}

s32 _DisableIntc(s32 cause) {
    return _iDisableIntc(cause);
}

s32 _EnableDmac(s32 channel) {
    s32 r = _iEnableDmac(channel);
    deliver();
    maybe_schedule();
    return r;
}

s32 _DisableDmac(s32 channel) {
    return _iDisableDmac(channel);
}

/* The libkernel wrappers (DIntr, the call, EIntr if they were on) */
s32 EnableIntc(s32 cause) {
    return _EnableIntc(cause);
}

s32 DisableIntc(s32 cause) {
    return _DisableIntc(cause);
}

s32 EnableDmac(s32 channel) {
    return _EnableDmac(channel);
}

s32 DisableDmac(s32 channel) {
    return _DisableDmac(channel);
}

/* Both return whether interrupts were on */
s32 DIntr(void) {
    s32 was;

    KINIT();
    was = irq_enabled;
    irq_enabled = 0;
    return was;
}

s32 EIntr(void) {
    s32 was;

    KINIT();
    was = irq_enabled;
    irq_enabled = 1;
    if (!was) {
        deliver();
        maybe_schedule();
    }
    return was;
}

/* ---------------------------------------------------------------------- */
/* Alarms: one-shot, `time` HSYNCs from now; the handler gets (id, time,
   arg) in interrupt context */

static s32 set_alarm(u16 time, void *fn, void *arg) {
    int i;

    KINIT();
    if (fn == NULL) {
        return -1;
    }
    for (i = 0; i < MAX_ALARMS; i++) {
        if (!alarms[i].used) {
            break;
        }
    }
    if (i == MAX_ALARMS) {
        return -1;
    }
    alarms[i].used = 1;
    alarms[i].due = hsync_now + time;
    alarms[i].time = time;
    alarms[i].fn = fn;
    alarms[i].arg = arg;
    alarms[i].seq = alarm_seq++;
    return i + 1;
}

s32 SetAlarm(u16 time, void *handler, void *arg) {
    return set_alarm(time, handler, arg);
}

s32 iSetAlarm(u16 time, void *handler, void *arg) {
    return set_alarm(time, handler, arg);
}

s32 ReleaseAlarm(s32 id) {
    KINIT();
    if (id <= 0 || id > MAX_ALARMS || !alarms[id - 1].used) {
        return -1;
    }
    alarms[id - 1].used = 0;
    return id;
}

s32 iReleaseAlarm(s32 id) {
    return ReleaseAlarm(id);
}

/* ---------------------------------------------------------------------- */
/* Caches: nothing to do on the host */

void FlushCache(s32 op) {
    (void)op;
}

void iFlushCache(s32 op) {
    (void)op;
}

void SyncDCache(void *start, void *end) {
    (void)start;
    (void)end;
}

void _SyncDCache(u32 start, u32 end) {
    (void)start;
    (void)end;
}

void _InvalidDCache(u32 start, u32 end) {
    (void)start;
    (void)end;
}

void Exit(s32 status) {
    grx_log("Exit(%d)", (int)status);
    exit(status);
}

/* ---------------------------------------------------------------------- */
/* The platform's side (grx_kernel.h) */

void grx_kernel_raise_intc(int cause) {
    KINIT();
    if (cause < 0 || cause >= NUM_CAUSES) {
        grx_log("kernel: raise of INTC cause %d", cause);
        return;
    }
    intc_pending |= 1u << cause;
    deliver();
    maybe_schedule();
}

void grx_kernel_raise_dmac(int channel) {
    KINIT();
    if (channel < 0 || channel >= NUM_CAUSES) {
        grx_log("kernel: raise of DMAC channel %d", channel);
        return;
    }
    dmac_pending |= 1u << channel;
    deliver();
    maybe_schedule();
}

void grx_kernel_hsync(int count) {
    unsigned long long target;

    KINIT();
    if (count < 0) {
        count = 0;
    }
    target = hsync_now + (unsigned)count;
    /* Step through the alarms that fall due, so an alarm a handler sets
       counts from when that handler ran */
    for (;;) {
        KAlarm *a = next_alarm();
        if (a == NULL || a->due > target || in_interrupt || !irq_enabled) {
            break;
        }
        if (a->due > hsync_now) {
            hsync_now = a->due;
        }
        deliver();
    }
    hsync_now = target;
    deliver();
    maybe_schedule();
}

void grx_kernel_field(void) {
    KINIT();
    hold++;
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START);
    grx_kernel_hsync(GRX_HSYNC_VBLANK);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_END);
    grx_kernel_hsync(GRX_HSYNC_PER_FIELD - GRX_HSYNC_VBLANK + (field_count & 1));
    field_count++;
    hold--;
    maybe_schedule();
}

void grx_kernel_set_idle_hook(void (*hook)(void)) {
    idle_hook = hook;
}

int grx_kernel_current_thread(void) {
    KINIT();
    return cur->id;
}

int grx_kernel_in_interrupt(void) {
    return in_interrupt;
}

unsigned long long grx_kernel_hsync_now(void) {
    return hsync_now;
}

void grx_kernel_dump(void) {
    static const char *const waits[] = {"", " sleep", " sema"};
    int i;

    KINIT();
    grx_log("kernel: thread %d running%s%s, interrupts %s, hsync %llu", cur->id,
            in_interrupt ? " (in a handler)" : "", idling ? " (idle)" : "", irq_enabled ? "on" : "off",
            hsync_now);
    for (i = 1; i < MAX_THREADS; i++) {
        KThread *t = &threads[i];
        if (t->used) {
            grx_log("  thread %3d prio %3d status %02X%s%s%d wakeup %d suspend %d entry %p", i, t->prio,
                    thread_status(t), waits[t->wait_type], t->wait_type == TSW_SEMA ? " " : "",
                    t->wait_type == TSW_SEMA ? t->wait_id : 0, t->wakeup, t->suspend, t->entry);
        }
    }
    for (i = 1; i < MAX_SEMAS; i++) {
        KSema *s = &semas[i];
        if (s->used) {
            grx_log("  sema %3d count %d max %d waiters %d", i, s->count, s->max, s->nwait);
        }
    }
    for (i = 0; i < MAX_HANDLERS; i++) {
        KHandler *h = &handlers[i];
        if (h->used) {
            u32 mask = h->dmac ? dmac_mask : intc_mask;
            grx_log("  handler %3d %s %2d %s %p", i + 1, h->dmac ? "dmac" : "intc", h->cause,
                    (mask >> h->cause) & 1 ? "enabled" : "masked", h->fn);
        }
    }
    for (i = 0; i < MAX_ALARMS; i++) {
        KAlarm *a = &alarms[i];
        if (a->used) {
            grx_log("  alarm %2d due %llu %p(%p)", i + 1, a->due, a->fn, a->arg);
        }
    }
    grx_log("  pending intc %04X (mask %04X) dmac %04X (mask %04X)", intc_pending, intc_mask, dmac_pending,
            dmac_mask);
}

/* ---------------------------------------------------------------------- */
/* Save states (grx_state_parts.h) */

/* Saved only with the main thread and the idle loop running: every other
   thread unused or dormant, the idle loop running on the main thread's
   context, outside interrupts and fields */
void *grx_kernel_main_wait_site(void) {
    return main_wait_site;
}

const char *grx_kernel_state_blocker(void) {
    int i;
    for (i = 2; i < MAX_THREADS; i++) {
        if (threads[i].used && !threads[i].dormant) {
            return "the game is running more than one thread";
        }
    }
    if (on_fiber != main_fiber || cur != &idle_thread || !idling || in_interrupt || hold || reap_count) {
        return "the game isn't between fields";
    }
    return NULL;
}

/* Before a load: free this run's parked contexts (threads that exited,
   which keep theirs to start again), each once; never main_fiber or the
   one this runs on (the blocker makes them the same) */
static void drop_fibers(void) {
    int i, j;
    for (i = 0; i <= MAX_THREADS; i++) {
        GrxFiber *f = i < MAX_THREADS ? threads[i].fiber : idle_thread.fiber;
        if (f == NULL || f == main_fiber || f == on_fiber) {
            continue;
        }
        grx_fiber_delete(f);
        for (j = 0; j < MAX_THREADS; j++) {
            if (threads[j].fiber == f) {
                threads[j].fiber = NULL;
            }
        }
        if (idle_thread.fiber == f) {
            idle_thread.fiber = NULL;
        }
    }
}

void grx_kernel_state_io(GrxState *s) {
    int i;
    if (grx_state_loading(s)) {
        /* While threads[] still holds this run's pointers */
        drop_fibers();
    }
    GRX_STATE_VAR(s, threads);
    GRX_STATE_VAR(s, idle_thread);
    GRX_STATE_VAR(s, cur);
    GRX_STATE_VAR(s, ready);
    GRX_STATE_VAR(s, semas);
    GRX_STATE_VAR(s, handlers);
    GRX_STATE_VAR(s, handler_seq_head);
    GRX_STATE_VAR(s, handler_seq_tail);
    GRX_STATE_VAR(s, alarms);
    GRX_STATE_VAR(s, alarm_seq);
    GRX_STATE_VAR(s, irq_enabled);
    GRX_STATE_VAR(s, intc_mask);
    GRX_STATE_VAR(s, intc_pending);
    GRX_STATE_VAR(s, dmac_mask);
    GRX_STATE_VAR(s, dmac_pending);
    GRX_STATE_VAR(s, hsync_now);
    GRX_STATE_VAR(s, field_count);
    if (grx_state_loading(s)) {
        /* Contexts are this run's: the main thread's is main_fiber; a
           dormant thread gets a new one when it starts (this run's parked
           ones are freed above) */
        for (i = 0; i < MAX_THREADS; i++) {
            threads[i].fiber = i == 1 ? main_fiber : NULL;
        }
        idle_thread.fiber = NULL;
        on_fiber = main_fiber;
    }
}
