#include "common.h"
#include "protos/field.h"
#include "field/event.h"

typedef struct FieldSrc {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6[7];
    /* 0x14 */ u16 unk14[32];
    /* 0x54 */ u16 unk54[32];
} FieldSrc;


void OccsLoad(FieldSrc *src, s32 n) {
    s32 i;
    s32 j;
    OccsEntry *d;
    FieldSrc *s;

    for (i = 0; i < n; i++) {
        s = &src[i];
        d = OccsGet(i);
        d->state = OCC_SLEEP;
        d->id = s->unk0;
        d->areaId = s->unk2;
        d->flags = s->unk4;
        for (j = 0; j < 7; j++) {
            ((u16 *)&d->cond)[j] = s->unk6[j];
        }
        for (j = 0; j < 32; j++) {
            d->cmds[j].op = s->unk14[j];
            d->cmds[j].arg = s->unk54[j];
        }
    }
    OccsSetCount(i);
}
