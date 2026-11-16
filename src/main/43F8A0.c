#include "common.h"
#include "gph.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0xEC];
    s32 unkEC;
    s32 unkF0;
} Work;

typedef struct {
    u8 pad[0x2C];
    s32 unk2C;
    s32 unk30;
} Stats;

void GsTexPacketSet(void *, GphTex *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void GsTexUpload(void *);

void GphTexBind(u8 id) {
    GphTex *src;
    Work *w;
    Stats *st;
    s32 f;

    if (id == 0) {
        return;
    }
    src = gphTexTable->slots[id].src;
    if (src == NULL) {
        return;
    }
    f = src->uploadMode ? 3 : 0;
    w = (Work *)0x70003000;
    GsTexPacketSet((void *)0x70003000, src, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, f);
    __asm__ volatile("sync");
    GsTexUpload((void *)0x70003000);
    GsTexPacketSend((void *)0x70003000);
    st = (Stats *)0x70002700;
    st->unk2C++;
    st->unk30 += w->unkEC * w->unkF0;
}
