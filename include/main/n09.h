#ifndef MAIN_N09_H
#define MAIN_N09_H

#include "common.h"

/* Display timing (0x0045E980-0x0045EC18): vblank counters kept by the
 * vblank interrupt handler GsVblankHandler (0x0045EB50) and the Timer 0
 * frame-time measurement. The counters change in the interrupt handler, so
 * they are volatile. */
extern volatile s32 gsVblanksSinceFlip;  /* 0x004DB8BC; cleared when a flip wait ends */
extern volatile s32 gsFlipWaitCount;     /* 0x004DB8C0; vblanks left to wait in GsVsyncFlip */
extern volatile s32 gsVblankCount;       /* 0x004DB8C4; vblanks since GsVsyncInit */
extern volatile s32 gsVblankIdleCount;   /* 0x004DB8C8; vblanks while no flip wait is pending */
extern u32 gsTimerTicksPerFrame;         /* 0x004DB8CC; T0_COUNT ticks in one frame */
extern u32 gsTimerLastCount;             /* 0x004DB8D0; last T0_COUNT read by GsGetFrameTimePercent */
extern s32 gsVblankHandlerId;            /* 0x004DB8D4; AddIntcHandler id of GsVblankHandler */

#endif
