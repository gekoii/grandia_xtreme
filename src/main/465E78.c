#include "common.h"
#include "protos/main.h"
#include "snd.h"

typedef struct {
    u8 b[4];
} Tag;

extern Tag movSeqEndCode;

s32 videoDecFlush(VideoDec *o) {
    Tag t;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 h;

    t = movSeqEndCode;
    videoDecBeginPut(o, &a, &b, &c, &d);
    if (b + d < 4) {
        return 0;
    }
    h = videoDecCopy2area((a & 0x0FFFFFFF) | 0x20000000, b, (c & 0x0FFFFFFF) | 0x20000000, d, &t, 4, 0, 0);
    videoDecEndPut(&movVideoDec, h);
    viBufFlush(&o->vibuf);
    if (o->state == 0) {
        o->state = 2;
    }
    return 1;
}
