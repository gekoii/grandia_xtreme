#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

extern s32 sndStrmUploadLastOfs;

s32 SndStrmUploadStart(s32 arg0, s32 arg1, s32 arg2) {
    s32 d;
    s16 a1;
    s16 a3;
    s32 n;

    d = sndIopStatus->doneSeq - sndStrmUploadSeq;
    if (d < -0x7FFF) {
        d += 0x10000;
    }
    if (d < 0) {
        return -0xA;
    }
    if (SndStrmUploadPoll(sndStrmUploadSeq) != 0) {
        return -1;
    }
    if (sndStrmUploadSeq == 0) {
        return -0xB;
    }
    a1 = sndIopStatus->strm[arg0].bufOfs;
    if (sndStrmUploadLastOfs == a1) {
        return -0xC;
    }
    a3 = sndIopStatus->strm[arg0].bufLen;
    sndStrmUploadSeq = 0;
    n = sndIopStatus->strm[arg0].secNum + arg2;
    if (a3 < n) {
        return -3;
    }
    n = a3 - a1;
    sndStrmUploadDmaDesc.data = arg1;
    sndStrmUploadLastOfs = a1;
    sndStrmUploadRemain = 0;
    sndStrmUploadDmaDesc.addr = sndIopStatus->strm[arg0].bufAdr + (a1 << 11);
    if (n < arg2) {
        arg2 -= n;
        sndStrmUploadSrc2 = arg1 + (n << 11);
        sndStrmUploadRemain = arg2;
    }
    sndStrmUploadDmaDesc.size = arg2 << 11;
    sndStrmUploadDmaDesc.mode = 0;
    sndStrmUploadDmaId = sceSifSetDma(&sndStrmUploadDmaDesc, 1);
    if (sndStrmUploadDmaId == 0) {
        return -2;
    }
    sndStrmUploadSlot = arg0;
    sndStrmUploadCount = arg2;
    return arg2;
}
