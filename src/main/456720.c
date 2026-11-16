#include "common.h"
#include "main/n08.h"

typedef struct {
    GsPacketHead head;
    u64 frame;
    u64 frameReg;
    u64 zbuf;
    u64 zbufReg;
    u64 scissor;
    u64 scissorReg;
    u64 xyoffset;
    u64 xyoffsetReg;
    u64 dthe;
    u64 dtheReg;
} DrawEnvPacket;

void GsDrawEnvPacketSet(DrawEnvPacket *p, u32 fbp, u32 fbw, u32 psm, u32 fbmsk, u32 zbp, u32 zpsm, u32 zmsk, u32 x0, u32 y0, u32 x1, u32 y1, u32 ox, u32 oy, u32 dthe) {
    p->head.dmaTag = 0x10000007;
    p->head.id = 0xEEEE0001;
    p->head.vif[3] = 0x50000006;
    p->head.gifTag[0] = 0x1000000000008005;
    p->head.gifTag[1] = 0xE;
    p->frame = (u64)(fbp >> 11) + ((u64)(fbw >> 6) << 16) + ((u64)psm << 24) + ((u64)fbmsk << 32);
    p->frameReg = 0x4C;
    p->zbuf = (u64)(zbp >> 11) + ((u64)zpsm << 24) + ((u64)zmsk << 32);
    p->zbufReg = 0x4E;
    p->scissor = (u64)x0 + ((u64)x1 << 16) + ((u64)y0 << 32) + ((u64)y1 << 48);
    p->scissorReg = 0x40;
    p->xyoffset = (0x8000 - (u64)ox) + ((0x8000 - (u64)oy) << 32);
    p->xyoffsetReg = 0x18;
    p->dthe = dthe;
    p->head.vif[0] = 0;
    p->head.vif[1] = 0;
    p->head.vif[2] = 0;
    p->dtheReg = 0x45;
    __asm__ volatile("sync");
}
