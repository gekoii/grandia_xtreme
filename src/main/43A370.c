#include "common.h"
#include "protos/main.h"
#include "main/n05.h"
#include "main/n07.h"


void GphShadowInit(void) {
    GsAdPkt *p;

    gphShadowColor[0] = -255.0f;
    gphShadowColor[1] = -255.0f;
    gphShadowColor[2] = -255.0f;
    gphShadowColor[3] = 255.0f;
    p = &gphShadowGsPkt;
    GsAdPktInit(p, 4);
    p->reg[0].addr = 0x4A;
    p->reg[1].data = (0x80ULL << 32) | 0x64;
    p->reg[1].addr = 0x42;
    p->reg[2].data = 0x7C803;
    p->reg[2].addr = 0x47;
    p->reg[3].data = 0x60;
    p->reg[0].data = 0;
    p->reg[3].addr = 0x1B;
    p = &gphShadowZbufPkt;
    GsAdPktInit(p, 1);
    p->reg[0].addr = 0x4E;
    p->reg[0].data = (u64)(gsZBufAddr >> 11) | (1ULL << 32);
}
