#include "common.h"
#include "main/n08.h"

void sceDmaSendN(sceDmaChan *, u32, s32);

void GsGifDmaSendN(u32 arg0, s32 arg1) {
    if (arg0 > 0x6FFFFFFF) {
        arg0 = (arg0 + 0x90000000) | 0x80000000;
    }
    sceDmaSendN(gsDmaChanGif, arg0, arg1);
}
