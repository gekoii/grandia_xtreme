#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndStrmFetchStart(u32 idx, u8 *buf, s32 n) {
    s32 d;
    s32 avail;
    s32 size;
    u8 *src;

    if (idx >= 4 || sndStrmFetchSlot >= 0) {
        return -2;
    }
    if (SndIopIsRecvDone() == 0) {
        return -0xF;
    }
    if (sndIopWaitSeq != 0) {
        d = sndIopStatus->doneSeq - sndIopWaitSeq;
        if (d < -0x7FFF) {
            d += 0x10000;
        }
        if ((u32)d > 0x7FFF) {
            return -0xA;
        }
    }
    if (sndIopStatus->strm[idx].secNum < n) {
        return -0xD;
    }
    if (n <= 0) {
        return -4;
    }
    src = (u8 *)sndIopStatus->strm[idx].bufAdr + (sndIopStatus->strm[idx].getOfs << 11);
    avail = sndIopStatus->strm[idx].bufLen - sndIopStatus->strm[idx].getOfs;
    if (avail < n) {
        size = avail << 11;
        sndStrmFetchWrapped = 1;
        sndStrmFetchCount = avail;
        sndStrmFetchRemain = n - avail;
        sndStrmFetchDst2 = buf + size;
    } else {
        avail = n;
        sndStrmFetchWrapped = 0;
        sndStrmFetchCount = avail;
        size = avail << 11;
        sndStrmFetchRemain = 0;
        sndStrmFetchDst2 = 0;
    }
    if (SndIopRecv(src, buf, size)) {
        return -0x100;
    }
    sndStrmFetchSlot = idx;
    return 0;
}
