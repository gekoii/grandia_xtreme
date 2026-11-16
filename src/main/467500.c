#include "common.h"
#include "snd.h"

s32 voBufCreate(VoBuf *f, s32 base, u8 *mem, s32 num) {
    s32 i;
    f->count = 0;
    f->data = base;
    f->size = num;
    f->write = 0;
    for (i = 0; i < num; i++) {
        f->tag[i] = (VoTag *)(mem + i * 0x36040);
        f->tag[i]->status = 0;
    }
    return 0;
}
