#include "common.h"
#include "gph.h"





void GsTexPacketSet(void *, GphTex *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 GphTexRegister(GphTex *src) {
    u32 i;
    GphTexSlot *e;
    s32 f;

    e = &gphTexTable->slots[1];
    for (i = 1; i < gphTexTable->capacity; i++, e++) {
        if (e->flag == 0xFF) {
            e->flag = i;
            e->src = src;
            e->savedMode = src->uploadMode;
            f = 3;
            if (src->uploadMode == 0) {
                f = 0;
            }
            GsTexPacketSet(e->pkt, src, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, f);
            __asm__ volatile("sync");
            gphTexTable->count++;
            return i;
        }
    }
    return -1;
}
