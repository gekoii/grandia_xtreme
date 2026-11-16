#include "common.h"
#include "protos/main.h"
#include "gph.h"


extern s32 gsFbAlphaPktReady;
extern GsAdPkt gsFbAlphaPkt;

void GsFillFbAlpha(void) {
    GsAdPkt *p = &gsFbAlphaPkt;

    if (gsFbAlphaPktReady == 0) {
        GsAdPktInit(p, 10);
        p->reg[0].data = 0x31401;
        p->reg[0].addr = 0x47;
        p->reg[1].data = 0x800000006AULL;
        p->reg[1].addr = 0x42;
        p->reg[2].data = 0x40;
        p->reg[2].addr = 0x1B;
        p->reg[3].data = 6;
        p->reg[3].addr = 0;
        p->reg[4].data = 0x3F80000080000000ULL;
        p->reg[4].addr = 1;
        p->reg[5].data = 0x80008000;
        p->reg[5].addr = 4;
        p->reg[6].data = 0x9C00A800;
        p->reg[6].addr = 4;
        p->reg[7].data = 0x7040B;
        p->reg[7].addr = 0x47;
        p->reg[8].data = 0x4000000044ULL;
        p->reg[8].addr = 0x42;
        p->reg[9].data = 0x78;
        p->reg[9].addr = 0x1B;
        gsFbAlphaPktReady = 1;
    }
    GsGifPacketSend((u16 *)p);
}
