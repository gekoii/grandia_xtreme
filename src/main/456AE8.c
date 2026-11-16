#include "common.h"
#include "main/n08.h"

typedef struct {
    GsPacketHead head;
    u64 unk30;
    u64 unk38;
    f32 unk40;
    f32 unk44;
    f32 unk48;
} Packet;

void GsFogPacketSet(Packet *p, u32 r, u32 g, u32 b, f32 x, f32 y, f32 z) {
    p->unk40 = x;
    p->unk44 = y;
    p->unk48 = z;
    p->head.dmaTag = 0x10000003;
    p->head.id = 0xEEEE0005;
    p->head.vif[3] = 0x50000002;
    p->head.gifTag[0] = 0x1000000000008001;
    p->head.gifTag[1] = 0xE;
    p->unk30 = (u64)r + ((u64)g << 8) + ((u64)b << 16);
    p->head.vif[0] = 0;
    p->head.vif[1] = 0;
    p->head.vif[2] = 0;
    p->unk38 = 0x3D;
    __asm__ volatile("sync");
}
