#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

typedef struct {
    u32 w;
    u32 h;
    u8 pad8[0x14];
    void *data;
    u8 pad20[0x58];
    s32 kind;
} Img;

s32 MemMove128(void *, void *, s32);
void *FldMemAlloc(s32);

void MapRippleLoad(void) {
    Img *img;
    void *buf;
    void *s;
    s32 i;
    u16 *q;

    mapRippleWork.count = 0;
    for (i = 0; i < 0x100; i++) {
        img = GphTexGetSourceRaw(i);
        if (img != NULL && img->kind != 0) {
            mapRippleWork.slot[mapRippleWork.count] = i;
            mapRippleWork.kind[mapRippleWork.count] = img->kind;
            buf = FldMemAlloc(img->w * img->h);
            MemMove128(buf, img->data, (img->w * img->h) >> 4);
            s = FldMemAlloc(GphRippleWorkSize(img->w, img->h));
            mapRippleWork.surf[mapRippleWork.count] = s;
            GphRippleCreate(img->w, img->h, buf, img->data, s);
            q = &mapRippleWork.timer[mapRippleWork.count];
            mapRippleWork.count++;
            *q = 0;
        }
    }
}
