/* rodata: 0x004A67C0-0x004A67D8 */
#include "common.h"
#include "gph.h"


void GsTexInfoInit(GphTex *t, void *pixels, void *clut, s32 w, s32 h) {
    s32 sizes[5] = { 64, 128, 256, 512, 1024 };
    s32 i;

    t->tw = 0;
    t->th = 0;
    for (i = 0; i < 5; i++) {
        if (sizes[i] >= w) {
            t->tw = i + 6;
            w = sizes[i];
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        if (sizes[i] >= h) {
            t->th = i + 6;
            h = sizes[i];
            break;
        }
    }
    t->unk40[0] = clut;
    t->psm = 20;
    t->unk38 = 1;
    t->size = (w * h) >> 1;
    t->unk3C = 0x40;
    t->data[0] = pixels;
    t->w = w;
    t->h = h;
    t->unk14 = 0;
}
