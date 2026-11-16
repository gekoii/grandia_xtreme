#include "common.h"
#include "protos/main.h"
#include "gph.h"

extern s32 gsZClearPktReady;
extern GsAdPkt gsZClearPkt;

void GsClearZBuffer(void) {
    GsAdPkt *p = &gsZClearPkt;

    if (gsZClearPktReady == 0) {
        GsAdPktInit(p, 6);
        p->reg[0].data = 0x32801;
        p->reg[0].addr = 0x47;
        p->reg[1].data = 6;
        p->reg[1].addr = 0;
        p->reg[2].data = 0x3F80000000000000;
        p->reg[2].addr = 1;
        p->reg[3].data = 0x80008000;
        p->reg[3].addr = 4;
        p->reg[4].data = 0x9C00A800;
        p->reg[4].addr = 4;
        p->reg[5].data = 0x7040B;
        p->reg[5].addr = 0x47;
        gsZClearPktReady = 1;
    }
    GsGifPacketSend((u16 *)p);
}
