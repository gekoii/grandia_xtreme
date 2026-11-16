/* cflags: -fno-schedule-insns */
/* without -fno-schedule-insns sched1 hoists the argument move (lh a1 / move a0,a1 instead of lh a0) */
#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

void SndIopPoll(void) {
    s32 diff;
    SndIopStatus *p = sndIopStatus;
    s16 x = sndIopWaitSeq;
    diff = p->doneSeq - x;
    if (SndIopIsCmdDmaDone(x) != 0) {
        SndIopCmdFlush();
    }
    if (SndIopIsStatusRecvDone() != 0) {
        SndIopStatusRecv();
    }
    diff &= 0xFFFF;
    if (diff < 0x2000) {
        sndIopWaitSeq = 0;
    }
}
