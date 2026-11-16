#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    u8 pad0[0x10];
    f32 pos[4];
    u8 pad20[0x10];
    f32 unk30;
    s32 unk34;
    s32 unk38;
} Sub;

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
    u8 padE[0x60 - 0xE];
    Sub *unk60;
} Obj;


void FldSpotSpawnMonsters(void) {
    s32 r;
    u32 i;
    Obj *o;
    Sub *s;

    r = MapSetFileGet();
    if (r == 0) {
        return;
    }
    for (i = 0; i < fldSpotWork->count; i++) {
        if (fldSpotWork->list[i].kind == 2) {
            o = FldMonsterSpawn(r, 0);
            s = o->unk60;
            s->unk30 = 2.0f;
            s->unk34 = 0;
            s->unk38 = 0;
            ChrTenantSetActive(o, 0);
            o->unkC ^= 0x20;
            fldSpotWork->list[i].chara = o;
            copyq(o->unk60->pos, fldSpotWork->list[i].model->pos);
            ChrTenantAreaCreateAround(o, fldSpotWork->list[i].model->pos);
        }
    }
}
