/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

#define VADD_XYZ(d, a, b) do { \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory"); } while (0)

typedef struct {
    s16 type;
    u8 pad2[5];
    u8 flags7;
    u8 pad8[0x30 - 0x8];
    f32 off[4];
} Model;

typedef struct {
    u8 pad0[0x10];
    f32 pos[4];
} Node;

typedef struct {
    u8 pad0[6];
    u16 unk6;
    s32 unk8;
    u16 unkC;
    u8 padE[0x30 - 0xE];
    f32 unk30;
    f32 unk34;
    u8 pad38[0x4C - 0x38];
    Model *unk4C;
    u8 pad50[4];
    s32 unk54;
    u8 pad58[0x5F - 0x58];
    u8 unk5F;
    Node *unk60;
    s32 unk64;
    u8 pad68[0x180 - 0x68];
    f32 vel[4];
    u8 pad190[0x224 - 0x190];
    u16 unk224;
    u8 pad226[2];
    u8 unk228[4];
} Obj;

void FldObjMoveCmdUpdate(Obj *, s32);
Model *MapCollMove(s32, Node *, f32 *, void *, f32, s32, s32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void ChrObjMove(Obj *o) {
    Node *n = o->unk60;
    Model *m;
    f32 len;

    if (o->unk64 != 0) {
        FldObjMoveCmdUpdate(o, o->unk64);
    }
    m = o->unk4C;
    if (m != NULL && m->type == 0x10 && (m->flags7 & 1)) {
        VADD_XYZ(n->pos, n->pos, m->off);
    }
    do { /* do-while(0): keeps the alignment nops and branch layout (both wraps) */
        if ((o->unk6 & 8) && (o->unkC & 4)) {
            if (o->unk4C != NULL) {
                o->vel[1] -= 8.166667f;
            } else {
                o->vel[1] -= 16.333334f;
            }
            if (o->vel[1] < -200.0f) {
                o->vel[1] = -200.0f;
            }
        }
    } while (0);
    do {
        len = fsqrt(o->vel[0] * o->vel[0] + o->vel[1] * o->vel[1] + o->vel[2] * o->vel[2]);
        if (!(len < 0.01f)) {
            if (!(o->unkC & 0x800)) {
                if (o->unk6 & 1) o->unk6 ^= 1;
                if (o->unk6 & 2) o->unk6 ^= 2;
                if (o->unk6 & 4) o->unk6 ^= 4;
            }
            if (!(o->unkC & 0x400)) {
                if (o->unk6 & 8) o->unk6 ^= 8;
            }
            if (!(o->unkC & 4)) {
                if (o->unk6 & 8) o->unk6 ^= 8;
            }
            o->unk4C = MapCollMove(MapCollModelGet(0), n, o->vel, o->unk228, o->unk34, o->unk224, o->unk6);
        }
        len = fsqrt(o->vel[0] * o->vel[0] + o->vel[1] * o->vel[1] + o->vel[2] * o->vel[2]);
        o->unk30 = len;
        o->unk54 &= 0xFFFF;
        VADD_XYZ(n->pos, n->pos, o->vel);
        if (!(o->unkC & 0x40)) {
            if (o->unk8 & 2) {
                o->unk5F = 3;
            } else if (o->unk8 & 0x80) {
                o->unk5F = 0xF;
            } else {
                len = fsqrt(o->vel[0] * o->vel[0] + o->vel[2] * o->vel[2]);
                if (o->unk8 & 8) {
                    if (len < 0.1f) o->unk5F = 6; else o->unk5F = 8;
                } else if (len < 0.1f) {
                    o->unk5F = 0;
                } else if (len < 11.99f) {
                    o->unk5F = 1;
                } else {
                    o->unk5F = 2;
                }
            }
        }
    } while (0);
    if (!(o->unkC & 0x40) && !(o->unk8 & 0x20)) {
        ChrMotSet(o, o->unk5F);
    }
}
