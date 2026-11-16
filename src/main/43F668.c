#include "common.h"
#include "gph.h"

void GsTexPacketSet(void *, GphTex *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 GphTexRegisterAt(GphTex *src, u8 id) {
    GphTexSlot *e;
    GphTexSlot *base = gphTexTable->slots;

    e = (GphTexSlot *)((u32)(id * 0x2C0) + (u32)base); /* scaled index before base (addu order) */
    if (e->flag != 0xFF) {
        return -1;
    }
    e->src = src;
    e->flag = id;
    e->savedMode = src->uploadMode;
    GsTexPacketSet(e->pkt, src, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, src->uploadMode ? 3 : 0);
    __asm__ volatile("sync");
    gphTexTable->count++;
    return id;
}
