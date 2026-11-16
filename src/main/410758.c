#include "common.h"

static inline u32 fix(u32 p, u32 base) {
    if (p < base) {
        p += base;
    }
    return p;
}
#define FIX(p, base) fix((u32)(p), (u32)(base))

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    s8 unk14;
} Sub410;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u8 padC[4];
    u32 unk10;
} Obj410;

typedef struct {
    u32 pad0[2];
    u32 objs[5];
} Ent410;

typedef struct {
    u8 pad0[0xA4];
    u32 unkA4;
    u32 unkA8;
    u32 ents;
    u8 padB0;
    u8 count;
} Hdr410;

void PtyEnemyDataRelocate(Hdr410 *h) {
    s32 i;
    s32 k;
    Ent410 *e;
    Obj410 *o;
    Sub410 *s;

    h->unkA4 = FIX(h->unkA4, h);
    h->unkA8 = FIX(h->unkA8, h);
    if (h->count != 0) {
        h->ents = FIX(h->ents, h);
        e = (Ent410 *)h->ents;
        for (i = h->count; i > 0; e++, i--) {
            for (k = 0; k < 5; k++) {
                if (e->objs[k] != 0 && e->objs[k] < (u32)h) {
                    o = (Obj410 *)(e->objs[k] + (u32)h);
                    e->objs[k] = (u32)o;
                    o->unk8 = FIX(o->unk8, h);
                    s = (Sub410 *)FIX(o->unk10, h);
                    o->unk10 = (u32)s;
                    if (s->unk14 > 0) {
                        s->unk10 = FIX(s->unk10, h);
                    }
                }
            }
        }
    }
}
