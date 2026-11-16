#include "common.h"
#include "main/n10.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Out46;

s32 SndStrmGetReady(u32 arg0, Out46 *arg1) {
    s32 ret;
    s32 d;

    if (arg0 >= 4U) {
        return -2;
    }
    ret = sndIopStatus->strm[arg0].secNum;
    if (sndIopWaitSeq != 0) {
        d = sndIopStatus->doneSeq - sndIopWaitSeq;
        if (d < -0x7FFF) {
            d += 0x10000;
        }
        if ((u32)d > 0x7FFF) {
            ret = 0;
        }
    }
    if (arg1 != NULL) {
        arg1->unk0 = ret;
        arg1->unk4 = 0;
        arg1->unk8 = sndIopStatus->strm[arg0].bufAdr;
        arg1->unkC = sndIopStatus->strm[arg0].bufOfs;
        arg1->unk10 = sndIopStatus->strm[arg0].bufLen;
    }
    return ret;
}
