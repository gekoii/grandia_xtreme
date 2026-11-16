#include "common.h"
#include "main/n10.h"

extern s8 sndIopCmdLock;

s32 SndIopCmdPush(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SndIopCmd *q;
    s32 n;

    if (sndIopCmdLock != 0 || sndIopCmdCount >= 0x18) {
        return 0;
    }
    sndIopCmdLock = 1;
    q = (SndIopCmd *)((sndIopCmdCount << 4) + (u32)sndIopCmdBufPtr);
    n = sndIopCmdCount + 1;
    q->seq = sndIopCmdSeq;
    q->cmd = arg0;
    q->arg0 = arg1;
    q->arg1 = arg2;
    q->arg2 = arg3;
    sndIopCmdBufPtr[n].cmd = 0;
    sndIopCmdCount = n;
    if (++sndIopCmdSeq == 0) {
        sndIopCmdSeq = 1;
    }
    sndIopCmdLock = 0;
    return 1;
}
