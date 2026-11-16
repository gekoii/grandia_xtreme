#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x20];
    s32 x20;
    s32 x24;
    s32 x28;
} Ent;

typedef struct {
    u8 pad0[0x8];
    s32 x8;
    u16 flags;
    u8 padE[0x2C - 0xE];
    s32 x2C;
    u8 pad30[0x48 - 0x30];
    f32 x48;
    u8 pad4C[0x60 - 0x4C];
    Ent *mdl;
    u8 pad64[0x180 - 0x64];
    FVECTOR x180;
    u8 pad190[0x1E4 - 0x190];
    u8 x1E4[0x20];
    u8 x204[0x20];
} Chara;

void MemSet(void *, s32, s32);

void ChrAllStateReset(void) {
    Chara *c;
    Ent *e;
    s32 i;

    for (i = 0; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (i == 0) {
            c->flags = 0xC8F;
        } else {
            c->flags = 0xC8E;
        }
        c->x8 = 4;
        c->x2C = 0;
        c->x48 = 1.0f;
        MemSet(c->x1E4, 0, 0x20);
        MemSet(c->x204, 0, 0x20);
        e = ChrModelFindPart(c->mdl, 0x280);
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(c->x180) : "memory");
        if (e != NULL) {
            e->x20 = 0;
            e->x24 = 0;
            e->x28 = 0;
        }
        if (FldMapIsNoKamae() == 1) {
            ChrExtraPartsModeSet(c, 0);
        }
    }
}
