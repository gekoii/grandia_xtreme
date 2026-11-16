/*
 * Tests for the port's EE kernel (port/src/kernel.c). A standalone program:
 *
 *   clang -target i686-w64-mingw32 -std=gnu11 -Wall -Iport/include -Iinclude \
 *       port/tests/kernel_test.c port/src/kernel.c port/src/fiber.c -o kernel_test.exe
 *
 * Each test records what runs, in order, in a trace and compares it with
 * what the EE kernel's scheduling gives. Exit status: the failure count.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_ee_names.h"
#include "sdk/eekernel.h"
#include "grx_kernel.h"

/* The kernel calls under test (the EE kernel's signatures) */
s32 CreateThread(ThreadParam *param);
s32 DeleteThread(s32 tid);
s32 StartThread(s32 tid, void *arg);
void ExitThread(void);
void ExitDeleteThread(void);
s32 TerminateThread(s32 tid);
s32 ChangeThreadPriority(s32 tid, s32 prio);
s32 RotateThreadReadyQueue(s32 prio);
s32 GetThreadId(void);
s32 ReferThreadStatus(s32 tid, ThreadParam *info);
s32 SleepThread(void);
s32 WakeupThread(s32 tid);
s32 _iWakeupThread(s32 tid);
s32 CancelWakeupThread(s32 tid);
s32 SuspendThread(s32 tid);
s32 ResumeThread(s32 tid);
s32 CreateSema(SemaParam *param);
s32 DeleteSema(s32 sid);
s32 SignalSema(s32 sid);
s32 iSignalSema(s32 sid);
s32 WaitSema(s32 sid);
s32 PollSema(s32 sid);
s32 ReferSemaStatus(s32 sid, SemaParam *info);
s32 AddIntcHandler(s32 cause, void *handler, s32 next);
s32 RemoveIntcHandler(s32 cause, s32 hid);
s32 AddDmacHandler(s32 channel, void *handler, s32 next);
s32 RemoveDmacHandler(s32 channel, s32 hid);
s32 EnableIntc(s32 cause);
s32 DisableIntc(s32 cause);
s32 EnableDmac(s32 channel);
s32 DisableDmac(s32 channel);
s32 DIntr(void);
s32 EIntr(void);
s32 SetAlarm(u16 time, void *handler, void *arg);
s32 ReleaseAlarm(s32 id);
void FlushCache(s32 op);
void SyncDCache(void *start, void *end);

/* The platform functions the kernel uses */
void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    fputs("FATAL: ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(100);
}

void grx_unimplemented(const char *name) {
    grx_fatal("unimplemented: %s", name);
}

/* ---------------------------------------------------------------------- */

static char trace[4096];
static int failures, checks;

static void T(const char *fmt, ...) {
    va_list ap;
    size_t n = strlen(trace);
    if (n != 0 && n < sizeof(trace) - 1) {
        trace[n++] = ' ';
    }
    va_start(ap, fmt);
    vsnprintf(trace + n, sizeof(trace) - n, fmt, ap);
    va_end(ap);
}

#define CHECK(cond)                                                              \
    do {                                                                         \
        checks++;                                                                \
        if (!(cond)) {                                                           \
            failures++;                                                          \
            fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                        \
    } while (0)

#define CHECK_TRACE(expected)                                                    \
    do {                                                                         \
        checks++;                                                                \
        if (strcmp(trace, expected) != 0) {                                      \
            failures++;                                                          \
            fprintf(stderr, "%s:%d: trace\n  got:      %s\n  expected: %s\n",    \
                    __FILE__, __LINE__, trace, expected);                        \
        }                                                                        \
        trace[0] = 0;                                                            \
    } while (0)

static u8 dummy_stack[0x800];

static s32 mk_thread(void *entry, int prio) {
    ThreadParam tp;
    memset(&tp, 0, sizeof(tp));
    tp.entry = entry;
    tp.stack = dummy_stack;
    tp.stackSize = sizeof(dummy_stack);
    tp.initPriority = prio;
    return CreateThread(&tp);
}

static s32 mk_sema(int init, int max) {
    SemaParam sp;
    memset(&sp, 0, sizeof(sp));
    sp.initCount = init;
    sp.maxCount = max;
    return CreateSema(&sp);
}

static int status_of(s32 tid) {
    return ReferThreadStatus(tid, NULL);
}

#define MAIN_PRIO 10
#define THS_RUN 0x01
#define THS_READY 0x02
#define THS_WAIT 0x04
#define THS_SUSPEND 0x08
#define THS_WAITSUSPEND 0x0C
#define THS_DORMANT 0x10

/* ---------------------------------------------------------------------- */
/* Priorities, preemption, FIFO within a priority, rotation */

static s32 semA, tidMain;

static void thA(void *arg) {
    T("A1");
    WaitSema(semA);
    T("A2");
}

static void thB(void *arg) {
    T("B1");
    WakeupThread(tidMain);
    T("B2");
}

static void thC(void *arg) {
    T("C1");
    RotateThreadReadyQueue(MAIN_PRIO);
    T("C2");
    SleepThread();
    T("C3");
}

static void test_priorities(void) {
    ThreadParam tp;
    SemaParam sp;
    s32 a, b, c;

    tidMain = GetThreadId();
    CHECK(tidMain == 1);
    CHECK(ReferThreadStatus(0, &tp) == THS_RUN);
    CHECK(tp.currentPriority == 0);
    CHECK(ChangeThreadPriority(0, MAIN_PRIO) == 0);
    CHECK(ReferThreadStatus(tidMain, &tp) == THS_RUN && tp.currentPriority == MAIN_PRIO);

    semA = mk_sema(0, 1);
    CHECK(semA > 0);
    a = mk_thread(thA, 5);
    b = mk_thread(thB, 20);
    c = mk_thread(thC, MAIN_PRIO);
    CHECK(a > 1 && b > 1 && c > 1);
    CHECK(status_of(a) == THS_DORMANT);

    StartThread(a, NULL); /* higher: runs now */
    T("M1");
    StartThread(b, NULL); /* lower: waits */
    T("M2");
    StartThread(c, NULL); /* equal: behind the main thread */
    T("M3");
    CHECK(status_of(c) == THS_READY);
    RotateThreadReadyQueue(MAIN_PRIO); /* C, which rotates back */
    T("M4");
    SignalSema(semA); /* A takes it and preempts, then exits */
    T("M5");
    CHECK(status_of(a) == THS_DORMANT);
    SleepThread(); /* C sleeps, B wakes the main thread, which preempts it */
    T("M6");
    WakeupThread(c); /* equal: no preemption */
    T("M7");
    RotateThreadReadyQueue(MAIN_PRIO);
    T("M8");
    ChangeThreadPriority(0, 30); /* now below B */
    T("M9");
    ChangeThreadPriority(0, MAIN_PRIO);
    CHECK_TRACE("A1 M1 M2 M3 C1 M4 A2 M5 C2 B1 M6 M7 C3 M8 B2 M9");

    /* Restart A; terminate it while it waits on the semaphore */
    CHECK(StartThread(a, NULL) == a);
    CHECK(StartThread(a, NULL) < 0); /* not dormant */
    CHECK(ReferThreadStatus(a, &tp) == THS_WAIT && tp.waitType == 2 && tp.waitId == semA);
    CHECK(ReferSemaStatus(semA, &sp) == semA && sp.numWaitThreads == 1);
    CHECK(DeleteThread(a) < 0); /* not dormant */
    CHECK(TerminateThread(a) == a);
    CHECK(ReferSemaStatus(semA, &sp) == semA && sp.numWaitThreads == 0 && sp.currentCount == 0);
    CHECK(TerminateThread(0) < 0); /* self */
    CHECK(DeleteThread(a) == a && DeleteThread(b) == b && DeleteThread(c) == c);
    CHECK(status_of(a) < 0);
    CHECK_TRACE("A1");

    /* Raising another thread above the running one preempts */
    a = mk_thread(thA, 40);
    StartThread(a, NULL);
    T("M");
    ChangeThreadPriority(a, 1);
    T("M");
    SignalSema(semA);
    DeleteThread(a);
    DeleteSema(semA);
    CHECK_TRACE("M A1 M A2");
}

/* ---------------------------------------------------------------------- */
/* Semaphores */

static s32 semW;

static void thW(void *arg) {
    int n = (int)(size_t)arg;
    s32 r;
    T("W%d+", n);
    r = WaitSema(semW);
    if (r == semW) {
        T("W%d-", n);
    } else {
        T("W%dx", n);
    }
}

static void test_semaphores(void) {
    SemaParam sp;
    s32 s, w[3];
    int i;

    s = mk_sema(2, 3);
    CHECK(PollSema(s) == s);
    CHECK(PollSema(s) == s);
    CHECK(PollSema(s) < 0);
    CHECK(SignalSema(s) == s && SignalSema(s) == s && SignalSema(s) == s);
    CHECK(SignalSema(s) < 0); /* past maxCount */
    CHECK(ReferSemaStatus(s, &sp) == s && sp.currentCount == 3 && sp.maxCount == 3 && sp.initCount == 2);
    CHECK(WaitSema(s) == s); /* doesn't block */
    CHECK(DeleteSema(s) == s);
    CHECK(PollSema(s) < 0 && SignalSema(s) < 0 && WaitSema(s) < 0 && DeleteSema(s) < 0);
    CHECK(mk_sema(-1, 1) < 0);

    /* Waiters are released first come, first served; deleting the
       semaphore releases the rest with an error */
    semW = mk_sema(0, 10);
    for (i = 0; i < 3; i++) {
        w[i] = mk_thread(thW, 5);
        StartThread(w[i], (void *)(size_t)(i + 1));
    }
    CHECK(ReferSemaStatus(semW, &sp) == semW && sp.numWaitThreads == 3);
    SignalSema(semW);
    SignalSema(semW);
    DeleteSema(semW);
    CHECK_TRACE("W1+ W2+ W3+ W1- W2- W3x");
    for (i = 0; i < 3; i++) {
        CHECK(status_of(w[i]) == THS_DORMANT);
        DeleteThread(w[i]);
    }
}

/* ---------------------------------------------------------------------- */
/* Sleep, wakeup counts, suspension */

static void thS(void *arg) {
    T("s+");
    SleepThread();
    T("s1");
    SleepThread();
    T("s2");
    SleepThread();
    T("s3");
}

static void thP(void *arg) {
    T("p+");
    SleepThread();
    T("p-");
}

static void test_sleep(void) {
    ThreadParam tp;
    s32 s, p;

    s = mk_thread(thS, 20);
    CHECK(WakeupThread(s) < 0); /* dormant */
    StartThread(s, NULL);
    CHECK(WakeupThread(s) == s && WakeupThread(s) == s);
    CHECK(ReferThreadStatus(s, &tp) == THS_READY && tp.wakeupCount == 2);
    ChangeThreadPriority(0, 30); /* S runs; two of its sleeps return at once */
    CHECK(ReferThreadStatus(s, &tp) == THS_WAIT && tp.waitType == 1 && tp.wakeupCount == 0);
    WakeupThread(s);
    ChangeThreadPriority(0, MAIN_PRIO);
    CHECK_TRACE("s+ s1 s2 s3");
    DeleteThread(s);

    /* Wakeup count of the running thread, and cancelling it */
    CHECK(WakeupThread(0) == tidMain && WakeupThread(0) == tidMain);
    CHECK(CancelWakeupThread(0) == 2);
    WakeupThread(0);
    CHECK(SleepThread() == tidMain); /* doesn't block */

    /* A suspended waiting thread isn't run when its wait ends */
    p = mk_thread(thP, 5);
    StartThread(p, NULL);
    CHECK(SuspendThread(p) == p);
    CHECK(status_of(p) == THS_WAITSUSPEND);
    WakeupThread(p);
    CHECK(status_of(p) == THS_SUSPEND);
    T("M");
    CHECK(ResumeThread(p) == p);
    CHECK(ResumeThread(p) < 0);
    CHECK_TRACE("p+ M p-");
    DeleteThread(p);
}

/* ---------------------------------------------------------------------- */
/* Interrupt and DMA handlers */

static s32 h1(s32 cause) { T("h1:%d", cause); return 0; }
static s32 h2(s32 cause) { T("h2"); return 0; }
static s32 h3(s32 cause) { T("h3"); return -1; }
static s32 h4(s32 cause) { T("h4"); return 0; }

static s32 semQ;

static s32 hq(s32 cause) {
    T("hq:%d:%d", GetThreadId(), grx_kernel_in_interrupt());
    iSignalSema(semQ);
    T("hq-");
    return 0;
}

static void thQ(void *arg) {
    for (;;) {
        WaitSema(semQ);
        T("Q");
    }
}

static s32 dh(s32 channel) { T("d%d", channel); return 0; }

static s32 nest_a(s32 cause) {
    T("a");
    grx_kernel_raise_intc(GRX_INTC_GS); /* latched until this chain ends */
    T("a-");
    return 0;
}

static s32 nest_b(s32 cause) { T("b"); return 0; }

static void thX(void *arg) {
    T("x:%d", DIntr()); /* this thread's own state: on */
    EIntr();
    WaitSema(semQ);
}

static void test_interrupts(void) {
    s32 id1, id2, id3, id4, idq, idd, ida, idb, q, x;

    id1 = AddIntcHandler(GRX_INTC_VBLANK_START, h1, 0);
    id2 = AddIntcHandler(GRX_INTC_VBLANK_START, h2, 0); /* 0: at the head */
    CHECK(id1 > 0 && id2 > 0 && id1 != id2);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START); /* masked: latched */
    CHECK_TRACE("");
    CHECK(EnableIntc(GRX_INTC_VBLANK_START) == 1);
    CHECK(EnableIntc(GRX_INTC_VBLANK_START) == 0);
    CHECK_TRACE("h2 h1:2");
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START);
    CHECK_TRACE("h2 h1:2");
    id3 = AddIntcHandler(GRX_INTC_VBLANK_START, h3, 0); /* stops the chain */
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START);
    CHECK_TRACE("h3");
    CHECK(RemoveIntcHandler(GRX_INTC_VBLANK_START, id3) == 2);
    CHECK(RemoveIntcHandler(GRX_INTC_VBLANK_START, id3) < 0);
    id4 = AddIntcHandler(GRX_INTC_VBLANK_START, h4, -1); /* at the tail */
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START);
    CHECK_TRACE("h2 h1:2 h4");

    /* DIntr latches, EIntr delivers */
    CHECK(DIntr() == 1);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_START); /* one status bit */
    CHECK(DIntr() == 0);
    CHECK_TRACE("");
    CHECK(EIntr() == 0);
    CHECK(EIntr() == 1);
    CHECK_TRACE("h2 h1:2 h4");
    RemoveIntcHandler(GRX_INTC_VBLANK_START, id1);
    RemoveIntcHandler(GRX_INTC_VBLANK_START, id2);
    RemoveIntcHandler(GRX_INTC_VBLANK_START, id4);
    DisableIntc(GRX_INTC_VBLANK_START);

    /* A handler readies a higher-priority thread: it runs on the way out of
       the interrupt, before raise returns */
    semQ = mk_sema(0, 255);
    q = mk_thread(thQ, 5);
    StartThread(q, NULL);
    idq = AddIntcHandler(GRX_INTC_VBLANK_END, hq, 0);
    EnableIntc(GRX_INTC_VBLANK_END);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_END);
    T("M");
    CHECK_TRACE("hq:1:1 hq- Q M");
    CHECK(DisableIntc(GRX_INTC_VBLANK_END) == 1);
    grx_kernel_raise_intc(GRX_INTC_VBLANK_END);
    CHECK_TRACE("");
    RemoveIntcHandler(GRX_INTC_VBLANK_END, idq);
    /* (cause 3 stays latched; nothing handles it now) */

    /* DMAC */
    idd = AddDmacHandler(GRX_DMAC_SIF0, dh, 0);
    CHECK(idd > 0);
    grx_kernel_raise_dmac(GRX_DMAC_SIF0);
    CHECK_TRACE("");
    EnableDmac(GRX_DMAC_SIF0);
    grx_kernel_raise_dmac(GRX_DMAC_SIF0);
    grx_kernel_raise_dmac(GRX_DMAC_GIF); /* no handler, masked */
    CHECK_TRACE("d5 d5");
    CHECK(DisableDmac(GRX_DMAC_SIF0) == 1);
    CHECK(RemoveDmacHandler(GRX_DMAC_SIF0, idd) == 0);
    CHECK(RemoveDmacHandler(GRX_DMAC_SIF0, idd) < 0);

    /* An interrupt raised inside a handler waits for the chain's end */
    ida = AddIntcHandler(GRX_INTC_VIF1, nest_a, 0);
    idb = AddIntcHandler(GRX_INTC_GS, nest_b, 0);
    EnableIntc(GRX_INTC_VIF1);
    EnableIntc(GRX_INTC_GS);
    grx_kernel_raise_intc(GRX_INTC_VIF1);
    CHECK_TRACE("a a- b");
    RemoveIntcHandler(GRX_INTC_VIF1, ida);
    RemoveIntcHandler(GRX_INTC_GS, idb);
    DisableIntc(GRX_INTC_VIF1);
    DisableIntc(GRX_INTC_GS);

    /* DIntr is per thread */
    CHECK(DIntr() == 1);
    x = mk_thread(thX, 5);
    StartThread(x, NULL);
    CHECK(DIntr() == 0);
    CHECK(EIntr() == 0);
    CHECK_TRACE("x:1");
    TerminateThread(x);
    DeleteThread(x);
    TerminateThread(q);
    DeleteThread(q);
    DeleteSema(semQ);
}

/* ---------------------------------------------------------------------- */
/* Alarms */

static unsigned long long alarm_base;

static void al(s32 id, u16 time, void *arg) {
    T("al%d@%llu:%d:%d", (int)(size_t)arg, grx_kernel_hsync_now() - alarm_base, GetThreadId(),
      grx_kernel_in_interrupt());
}

static int rearm_left;

static void al_rearm(s32 id, u16 time, void *arg) {
    T("r@%llu", grx_kernel_hsync_now() - alarm_base);
    if (--rearm_left > 0) {
        SetAlarm(10, al_rearm, NULL);
    }
}

static void test_alarms(void) {
    s32 id;

    alarm_base = grx_kernel_hsync_now();
    id = SetAlarm(100, al, (void *)7);
    CHECK(id > 0);
    grx_kernel_hsync(50);
    CHECK_TRACE("");
    grx_kernel_hsync(60);
    CHECK_TRACE("al7@100:1:1");
    CHECK(ReleaseAlarm(id) < 0); /* one-shot: gone */

    /* An alarm set by an alarm handler counts from when it ran */
    alarm_base = grx_kernel_hsync_now();
    rearm_left = 3;
    SetAlarm(10, al_rearm, NULL);
    grx_kernel_hsync(35);
    CHECK_TRACE("r@10 r@20 r@30");

    /* In order of when they fall due */
    alarm_base = grx_kernel_hsync_now();
    SetAlarm(20, al, (void *)2);
    SetAlarm(10, al, (void *)1);
    id = SetAlarm(15, al, (void *)9);
    CHECK(ReleaseAlarm(id) == id);
    grx_kernel_hsync(30);
    CHECK_TRACE("al1@10:1:1 al2@20:1:1");

    /* Interrupts off: the alarm waits for EIntr */
    alarm_base = grx_kernel_hsync_now();
    DIntr();
    SetAlarm(5, al, (void *)3);
    grx_kernel_hsync(10);
    CHECK_TRACE("");
    EIntr();
    CHECK_TRACE("al3@10:1:1");
}

/* ---------------------------------------------------------------------- */
/* The idle loop */

static void alarm_signal(s32 id, u16 time, void *arg) {
    T("sig:%d", GetThreadId()); /* the idle loop, thread 0, was interrupted */
    iSignalSema((s32)(size_t)arg);
}

static void alarm_wake(s32 id, u16 time, void *arg) {
    T("wake:%d", GetThreadId());
    _iWakeupThread((s32)(size_t)arg);
}

static s32 semV;
static int hook_calls;

static s32 vblank_signal(s32 cause) {
    iSignalSema(semV);
    return 0;
}

static void hook(void) {
    hook_calls++;
    CHECK(GetThreadId() == 0);
    grx_kernel_field();
}

static void test_idle(void) {
    unsigned long long t0;
    s32 s, idv;
    int i;

    /* _sceCdSleep: wait on a semaphore an alarm signals; with nothing else
       to run, the idle loop skips ahead to the alarm */
    t0 = grx_kernel_hsync_now();
    s = mk_sema(0, 1);
    SetAlarm(1000, alarm_signal, (void *)(size_t)s);
    CHECK(WaitSema(s) == s);
    CHECK(grx_kernel_hsync_now() - t0 == 1000);
    DeleteSema(s);
    /* _sceMcSleep: SleepThread, woken by an alarm */
    SetAlarm(500, alarm_wake, (void *)(size_t)GetThreadId());
    CHECK(SleepThread() == tidMain);
    CHECK(grx_kernel_hsync_now() - t0 == 1500);
    CHECK_TRACE("sig:0 wake:0");

    /* The platform's hook drives fields while everything waits */
    semV = mk_sema(0, 255);
    idv = AddIntcHandler(GRX_INTC_VBLANK_START, vblank_signal, 0);
    EnableIntc(GRX_INTC_VBLANK_START);
    grx_kernel_set_idle_hook(hook);
    t0 = grx_kernel_hsync_now();
    for (i = 0; i < 3; i++) {
        WaitSema(semV);
    }
    CHECK(hook_calls == 3);
    CHECK(grx_kernel_hsync_now() - t0 == 3 * 262 + 1); /* 262, 263, 262 */
    grx_kernel_set_idle_hook(NULL);
    /* Without one, a vblank handler gets simulated fields */
    WaitSema(semV);
    CHECK(hook_calls == 3);
    RemoveIntcHandler(GRX_INTC_VBLANK_START, idv);
    DisableIntc(GRX_INTC_VBLANK_START);
    DeleteSema(semV);
}

/* ---------------------------------------------------------------------- */
/* grx_kernel_field: threads woken by its interrupts run after all of it */

static s32 semF;

static s32 vs(s32 cause) { T("vs"); iSignalSema(semF); return 0; }
static s32 ve(s32 cause) { T("ve"); return 0; }

static void thF(void *arg) {
    for (;;) {
        WaitSema(semF);
        T("F");
    }
}

static void test_field(void) {
    s32 f, i1, i2;

    semF = mk_sema(0, 1);
    f = mk_thread(thF, 5);
    StartThread(f, NULL);
    i1 = AddIntcHandler(GRX_INTC_VBLANK_START, vs, 0);
    i2 = AddIntcHandler(GRX_INTC_VBLANK_END, ve, 0);
    EnableIntc(GRX_INTC_VBLANK_START);
    EnableIntc(GRX_INTC_VBLANK_END); /* runs ve: cause 3 was latched earlier */
    CHECK_TRACE("ve");
    grx_kernel_field();
    T("M");
    CHECK_TRACE("vs ve F M");
    RemoveIntcHandler(GRX_INTC_VBLANK_START, i1);
    RemoveIntcHandler(GRX_INTC_VBLANK_END, i2);
    DisableIntc(GRX_INTC_VBLANK_START);
    DisableIntc(GRX_INTC_VBLANK_END);
    TerminateThread(f);
    DeleteThread(f);
    DeleteSema(semF);
}

/* ---------------------------------------------------------------------- */
/* Exits, restarts, and many contexts created and freed */

static int ran;

static void th_return(void *arg) {
    ran += (int)(size_t)arg;
}

static void th_exit(void *arg) {
    ran++;
    ExitThread();
    ran += 1000;
}

static void th_exit_delete(void *arg) {
    ran++;
    ExitDeleteThread();
    ran += 1000;
}

static s32 semZ;

static void th_block(void *arg) {
    ran++;
    WaitSema(semZ);
    ran += 1000;
}

static void th_deep(void *arg) {
    /* Uses a good part of a fiber's stack, far more than the 2 KB the
       game gives some threads */
    volatile char buf[256 * 1024];
    buf[0] = 1;
    buf[sizeof(buf) - 1] = 2;
    ran += buf[0] + buf[sizeof(buf) - 1];
}

static void test_lifecycle(void) {
    s32 t, maxid = 0;
    int i;

    t = mk_thread(th_return, 5);
    StartThread(t, (void *)3);
    StartThread(t, (void *)4); /* restarted with a new argument */
    CHECK(ran == 7);
    CHECK(DeleteThread(t) == t);

    ran = 0;
    semZ = mk_sema(0, 1);
    for (i = 0; i < 3000; i++) {
        switch (i % 4) {
        case 0:
            t = mk_thread(th_return, 5);
            StartThread(t, (void *)1);
            DeleteThread(t);
            break;
        case 1:
            t = mk_thread(th_exit, 5);
            StartThread(t, NULL);
            DeleteThread(t);
            break;
        case 2:
            t = mk_thread(th_exit_delete, 5);
            StartThread(t, NULL);
            CHECK(status_of(t) < 0);
            break;
        case 3:
            t = mk_thread(th_block, 5);
            StartThread(t, NULL);
            TerminateThread(t);
            DeleteThread(t);
            break;
        }
        if (t > maxid) {
            maxid = t;
        }
    }
    CHECK(ran == 3000);
    CHECK(maxid == 2); /* ids reused */
    DeleteSema(semZ);

    ran = 0;
    t = mk_thread(th_deep, 5);
    StartThread(t, NULL);
    CHECK(ran == 3);
    DeleteThread(t);

    CHECK(mk_thread(NULL, 5) < 0);
    CHECK(mk_thread(th_return, 128) < 0);
    CHECK(StartThread(99, NULL) < 0);
    CHECK(ChangeThreadPriority(0, 128) < 0);
}

/* ---------------------------------------------------------------------- */

static void test_misc(void) {
    FlushCache(0);
    FlushCache(2);
    SyncDCache(dummy_stack, dummy_stack + sizeof(dummy_stack));
}

/* The game's movie threads: three at one priority, each yielding with
   RotateThreadReadyQueue (switchThread) */
static int rr_turns[3];

static void th_rr(void *arg) {
    int n = (int)(size_t)arg;
    int i;
    for (i = 0; i < 3; i++) {
        T("%c%d", 'a' + n, i);
        rr_turns[n]++;
        RotateThreadReadyQueue(MAIN_PRIO);
    }
}

static void test_round_robin(void) {
    s32 t1 = mk_thread(th_rr, MAIN_PRIO), t2 = mk_thread(th_rr, MAIN_PRIO);
    int i;
    StartThread(t1, (void *)1);
    StartThread(t2, (void *)2);
    for (i = 0; i < 4; i++) {
        T("m%d", i);
        RotateThreadReadyQueue(MAIN_PRIO);
    }
    CHECK_TRACE("m0 b0 c0 m1 b1 c1 m2 b2 c2 m3");
    DeleteThread(t1);
    DeleteThread(t2);
}

/* `kernel_test deadlock`: every thread waits and nothing can wake them;
   the kernel has to stop with a dump (exit status 100, from grx_fatal) */
static void th_stuck(void *arg) {
    WaitSema((s32)(size_t)arg);
}

static int run_deadlock(void) {
    s32 s = mk_sema(0, 1);
    StartThread(mk_thread(th_stuck, 5), (void *)(size_t)s);
    WaitSema(s);
    printf("deadlock not detected\n");
    return 1;
}

int main(int argc, char **argv) {
    struct {
        const char *name;
        void (*fn)(void);
    } tests[] = {
        {"priorities", test_priorities}, {"semaphores", test_semaphores}, {"sleep", test_sleep},
        {"interrupts", test_interrupts}, {"alarms", test_alarms},         {"idle", test_idle},
        {"field", test_field},           {"lifecycle", test_lifecycle},   {"round robin", test_round_robin},
        {"misc", test_misc},
    };
    size_t i;

    if (argc > 1 && strcmp(argv[1], "deadlock") == 0) {
        return run_deadlock();
    }
    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        int before = failures;
        trace[0] = 0;
        tests[i].fn();
        printf("%-12s %s\n", tests[i].name, failures == before ? "ok" : "FAILED");
    }
    printf("%d checks, %d failed\n", checks, failures);
    return failures;
}
