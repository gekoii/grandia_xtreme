#include "common.h"
typedef struct { u32 count; s32 pad[2]; s32 off; } Hdr;
void MiniTableRelocate(Hdr *h) {
    u32 i;
    s32 *p = (s32 *)(h->off + (s32)h);
    for (i = 0; i < h->count; i++) {
        p[i] += (s32)h;
    }
}
