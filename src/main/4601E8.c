#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndStrmFetchPoll(u32 arg0) {
    s32 i;

    if (arg0 >= 4U) {
        return -2;
    }
    if (arg0 != sndStrmFetchSlot) {
        return -8;
    }
    if (SndIopIsRecvDone() != 1) {
        return 1;
    }
    if (sndStrmFetchWrapped != 0) {
        sndIopWaitSeq = sndIopCmdSeq;
        while (SndIopCmdPush(0x110, sndStrmFetchSlot, sndStrmFetchCount, 1) == 0) {
            SndIopPoll();
            i = 0x18F;
            do {
                i -= 1;
            } while (i >= 0);
        }
        if (SndIopRecv(sndIopStatus->strm[arg0].bufAdr, (s32)sndStrmFetchDst2, sndStrmFetchRemain << 11) != 0) {
            return -0x101;
        }
        sndStrmFetchWrapped = 0;
        sndStrmFetchCount = sndStrmFetchRemain;
        return 2;
    }
    sndIopWaitSeq = sndIopCmdSeq;
    while (SndIopCmdPush(0x110, sndStrmFetchSlot, sndStrmFetchCount, 0) == 0) {
        SndIopPoll();
        i = 0x18F;
        do {
            i -= 1;
        } while (i >= 0);
    }
    sndStrmFetchSlot = -0x10;
    return 0;
}
