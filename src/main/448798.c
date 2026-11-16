#include "common.h"
#include "main/n07.h"

extern void MemSet(void *, s32, s32);
extern void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                          s32, s32, s32, s32, s32, s32, s32, s32, s64);

void GphBatchInit(void) {
    s32 i;

    MemSet(&gphBatchPackets, 0, 0xE80);
    gphBatchOffsetX = 0;
    gphBatchOffsetY = 0;
    for (i = 0; i < 2; i++) {
        gphBatchPackets.quad[i].h.dmaTag = 0x10000009;
        gphBatchPackets.quad[i].h.dmaTagHi = 0;
        gphBatchPackets.quad[i].h.gifRegs = 0xF5315315315310ULL;
        gphBatchPackets.quad[i].h.vif[0] = 0;
        gphBatchPackets.quad[i].h.vif[1] = 0;
        gphBatchPackets.quad[i].h.vif[2] = 0;
        gphBatchPackets.quad[i].h.vif[3] = 0x50000008;
        gphBatchPackets.quad[i].h.gifTag = 0xE400000000008001ULL;
        gphBatchPackets.quad[i].h.prim = 4;
    }
    for (i = 0; i < 2; i++) {
        gphBatchPackets.line[i].h.dmaTag = 0x10000005;
        gphBatchPackets.line[i].h.dmaTagHi = 0;
        gphBatchPackets.line[i].h.gifRegs = 0xF51510;
        gphBatchPackets.line[i].h.vif[0] = 0;
        gphBatchPackets.line[i].h.vif[1] = 0;
        gphBatchPackets.line[i].h.vif[2] = 0;
        gphBatchPackets.line[i].h.vif[3] = 0x50000004;
        gphBatchPackets.line[i].h.gifTag = 0x6400000000008001ULL;
        gphBatchPackets.line[i].h.prim = 1;
    }
    for (i = 0; i < 2; i++) {
        gphBatchPackets.sprite[i].h.dmaTag = 0x10000005;
        gphBatchPackets.sprite[i].h.dmaTagHi = 0;
        gphBatchPackets.sprite[i].h.gifRegs = 0xF535310;
        gphBatchPackets.sprite[i].h.vif[0] = 0;
        gphBatchPackets.sprite[i].h.vif[1] = 0;
        gphBatchPackets.sprite[i].h.vif[2] = 0;
        gphBatchPackets.sprite[i].h.vif[3] = 0x50000004;
        gphBatchPackets.sprite[i].h.gifTag = 0x6400000000008001ULL;
        gphBatchPackets.sprite[i].h.prim = 6;
    }
    GsRenderStatePacketSet(&gphBatchBlendStates[0], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 6, 128, 1, 0, 0, 1, 1, 0, 1, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[1], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 1, 0, 2, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[2], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 1, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[3], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 1, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[4], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 0, 1, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[5], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 0, 2, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[6], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[8], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 0, 1, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[9], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 0, 2, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[10], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 1, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[12], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 6, 128, 1, 0, 0, 1, 2, 0, 1, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[13], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 3, 0, 2, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[14], 1, 1, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 3, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[16], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 3, 0, 1, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[17], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 0, 128, 1, 0, 0, 1, 3, 0, 2, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    GsRenderStatePacketSet(&gphBatchBlendStates[18], 1, 0, 0, 1, 0, 1, 0,
                  0, 1, 5, 0, 1, 0, 0, 1, 3, 2, 0, 0,
                  1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
}
