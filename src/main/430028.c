#include "common.h"
#include "main/n03.h"

void MemSet(void *, s32, s32);
void GsDrawEnvPacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32);
void GsRenderStatePacketSet(void *, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s64);


void SysSetClearColor(u32 color) {
    SysClearPkt *p = &sysClearPkt;

    MemSet(p, 0, sizeof(SysClearPkt));
    GsDrawEnvPacketSet(sysClearDrawEnv, 0, 0x280, 0, 0, 0, 0, 0, 0, 0, 0x27F, 0x1BF, 0, 0, 0);
    GsRenderStatePacketSet(sysClearGsState, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0);
    p->dmaTag = 0x10000004;
    p->vif0 = 0;
    p->vif1 = 0;
    p->vif2 = 0;
    p->dmaTagHi = 0;
    p->direct = 0x50000003;
    p->gifTag = 0x4400000000008001ULL;
    p->gifRegs = 0x4410;
    p->prim = 6;
    p->rgbaq = (u64)color | 0x3F80000000000000ULL;
    p->xyz0 = 0x80008000;
    p->xyz1 = 0x9C00A800;
}
