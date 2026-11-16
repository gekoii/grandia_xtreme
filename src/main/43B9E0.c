#include "common.h"
#include "main/n05.h"

void GsAdPktInit(GsAdPkt *arg0, s32 arg1) {
    arg0->dmaTag = (u32)((arg1 + 2) | 0x10000000);
    arg0->gifRegs = 0xE;
    arg0->direct = ((arg1 + 1) & 0xFFFF) | 0x50000000;
    arg0->gifTag = (u64)(u32)(arg1 | 0x8000) | (0x8002ULL << 45);
    arg0->vif[0] = 0;
    arg0->vif[1] = 0;
    arg0->vif[2] = 0;
    arg0->dmaTagHi = 0;
}
