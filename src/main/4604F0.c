#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndStrmUploadPoll(s16 arg0) {
    if (sndStrmUploadRemain > 0) {
        if (sndStrmUploadDmaId != 0) {
            if (sceSifDmaStat(sndStrmUploadDmaId) < 0) {
                if (SndIopCmdPush(0x160, sndStrmUploadSlot, sndStrmUploadCount, 0) <= 0) {
                    return -3;
                }
                sndStrmUploadDmaDesc.data = sndStrmUploadSrc2;
                sndStrmUploadSeq = sndIopCmdSeq;
                sndStrmUploadDmaDesc.addr = sndIopStatus->strm[sndStrmUploadSlot].bufAdr;
                sndStrmUploadDmaDesc.size = sndStrmUploadRemain << 11;
                sndStrmUploadDmaDesc.mode = 0;
                sndStrmUploadDmaId = sceSifSetDma(&sndStrmUploadDmaDesc, 1);
                if (sndStrmUploadDmaId == 0) {
                    return -2;
                }
                sndStrmUploadRemain = 0;
                sndStrmUploadCount = 0;
            }
        }
    } else {
        if (sndStrmUploadDmaId == 0) {
            return 0;
        }
        if (sceSifDmaStat(sndStrmUploadDmaId) >= 0) {
            return 1;
        }
        if (SndIopCmdPush(0x160, sndStrmUploadSlot, sndStrmUploadCount, 0) <= 0) {
            return -4;
        }
        sndStrmUploadDmaId = 0;
        sndStrmUploadSeq = sndIopCmdSeq;
        return 0;
    }
    return 1;
}
