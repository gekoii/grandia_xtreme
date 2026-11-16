#include "common.h"
#include "main/n10.h"

s32 FlushCache(s32);
extern s32 sndIopCmdAddr;
extern s16 sndIopCmdBatchSeq;
extern sceSifDmaData sndIopCmdDmaDesc;

s32 SndIopCmdFlush(void) {
    if (sndIopCmdDmaBusy == 1 && sndIopStatus->batchSeq != sndIopCmdBatchSeq) {
        return 0;
    }
    if (sndIopCmdCount > 0) {
        FlushCache(0);
        sndIopCmdDmaBusy = 0;
        sndIopCmdDmaDesc.data = (u32)sndIopCmdBufPtr;
        sndIopCmdDmaDesc.addr = sndIopCmdAddr;
        sndIopCmdDmaDesc.size = 0x200;
        sndIopCmdDmaDesc.mode = 0;
        sndIopCmdDmaId = sceSifSetDma(&sndIopCmdDmaDesc, 1);

        if (sndIopCmdDmaId != 0) {
            sndIopCmdBatchSeq = sndIopCmdBufPtr->seq;
            sndIopCmdDmaBusy = 1;
            sndIopCmdCount = 0;
            sndIopStringCmdPending = 0;
            return 1;
        }
    }
    return 0;
}
