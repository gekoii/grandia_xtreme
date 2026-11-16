/* The port's EE kernel (port/src/kernel.c): what the platform layer calls
   to drive it. The kernel calls themselves (CreateThread, WaitSema, ...)
   have the game's names and are not declared here, so this header can be
   included next to <windows.h>.

   The kernel is cooperative and single-OS-threaded: every function here
   must be called on the OS thread that runs the game (from a game thread,
   or from the idle hook), never from an audio or input callback thread. */
#ifndef GRX_KERNEL_H
#define GRX_KERNEL_H

/* INTC causes (AddIntcHandler, EnableIntc) */
#define GRX_INTC_GS 0
#define GRX_INTC_SBUS 1
#define GRX_INTC_VBLANK_START 2
#define GRX_INTC_VBLANK_END 3
#define GRX_INTC_VIF0 4
#define GRX_INTC_VIF1 5
#define GRX_INTC_VU0 6
#define GRX_INTC_VU1 7
#define GRX_INTC_IPU 8
#define GRX_INTC_TIM0 9
#define GRX_INTC_TIM1 10
#define GRX_INTC_TIM2 11
#define GRX_INTC_TIM3 12
#define GRX_INTC_SFIFO 13
#define GRX_INTC_VU0WD 14

/* DMAC channels (AddDmacHandler, EnableDmac) */
#define GRX_DMAC_VIF0 0
#define GRX_DMAC_VIF1 1
#define GRX_DMAC_GIF 2
#define GRX_DMAC_FROM_IPU 3
#define GRX_DMAC_TO_IPU 4
#define GRX_DMAC_SIF0 5
#define GRX_DMAC_SIF1 6
#define GRX_DMAC_SIF2 7
#define GRX_DMAC_FROM_SPR 8
#define GRX_DMAC_TO_SPR 9

/* HSYNCs per NTSC field, and how many of them the vertical blank lasts */
#define GRX_HSYNC_PER_FIELD 262
#define GRX_HSYNC_VBLANK 22

/* Optional: the kernel initialises itself on first use. The calling OS
   thread becomes the game's main thread (thread 1). */
void grx_kernel_init(void);

/* An interrupt: runs the cause's (channel's) handlers in order if it is
   enabled and interrupts are on, else latches it until they are (as the
   INTC/DMAC status bits do). Then, called from a game thread, it switches
   to a higher-priority thread the handlers made ready, as the kernel does
   on return from an interrupt. */
void grx_kernel_raise_intc(int cause);
void grx_kernel_raise_dmac(int channel);

/* Advance the alarm clock (SetAlarm) by `count` HSYNCs, firing the alarms
   that fall due, in order */
void grx_kernel_hsync(int count);

/* One NTSC field: VBLANK_START, the vertical blank's HSYNCs, VBLANK_END,
   the rest of the field's HSYNCs (262 or 263 in all, alternately). Threads
   the handlers wake run after all of it. The frame loop calls this once
   per field (59.94 Hz). */
void grx_kernel_field(void);

/* Called when every thread is waiting. It has to make something ready
   (typically: present, wait for the next field in real time, then
   grx_kernel_field()); it is called again until something is. Without a
   hook, the kernel fast-forwards to the next alarm, or else simulates a
   field if a vblank handler is enabled, or else stops: deadlock. */
void grx_kernel_set_idle_hook(void (*hook)(void));

/* The running thread's id (0: the idle loop), and whether an interrupt
   handler is running */
int grx_kernel_current_thread(void);
int grx_kernel_in_interrupt(void);

/* HSYNCs counted so far */
unsigned long long grx_kernel_hsync_now(void);

/* Log the thread, semaphore, handler and alarm tables */
void grx_kernel_dump(void);

#endif
