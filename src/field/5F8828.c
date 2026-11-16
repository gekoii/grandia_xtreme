/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct Node {
    s16 id;
    s8 b2;
    s8 type;
    s8 state;
    u8 flags;
    u8 pad6[0xA];
    FVECTOR pos;
    f32 spd;
    u8 pad24[0xC];
    struct Node *next;
} Node;

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
} Obj;

typedef struct {
    u8 pad0[0xC];
    u16 flags;
    u8 padE[0x8];
    s8 unk16;
    u8 pad17[0x11];
    u32 cnt;
    u8 pad2C[0x4];
    f32 unk30;
    u8 pad34[0x18];
    s32 unk4C;
    u8 pad50[0x4];
    s32 unk54;
    u8 pad58[0x8];
    Obj *obj;
    u8 pad64[0x11C];
    FVECTOR vel;
} Owner;

extern f32 fldMoveJumpVelX;
extern f32 fldMoveJumpVelY;
extern f32 fldMoveJumpVelZ;

f32 VuVecDot(f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define VSUB(d, a, b) \
    __asm__ volatile("lqc2 $vf1, 0(%1)\n\tlqc2 $vf2, 0(%2)\n\tvsub.xyz $vf1, $vf1, $vf2\n\tsqc2 $vf1, 0(%0)" \
                     : : "r"(d), "r"(a), "r"(b) : "memory")
#define VUNIT(d) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(d) : "memory")

void FldObjMoveCmdUpdate(Owner *o, Node *n)
{
    FVECTOR d;
    f32 spd;
    f32 sq;
    f32 len;
    f32 t;
    f32 k;
    f32 inv;
    f32 l1;
    f32 *p;

    if (n == NULL) {
        return;
    }
    for (;;) {
        switch (n->type) {
        case 2:
            if (n->flags & 0x10) {
                if (!(o->flags & 4)) {
                    n->state = 3;
                } else if (n->state == 0) {
                    n->state++;
                    p = n->pos;
                    spd = n->spd;
                    VSUB(d, p, o->obj->pos);
                    d[1] = 0.0f;
                    sq = VuVecDot(d, d);
                    l1 = fsqrt(sq);
                    if (l1 < 1.0f) {
                        n->state = 3;
                    }
                    t = l1 / spd;
                    k = 1.0f / sq * spd;
                    if (t >= 1.0f) {
                        fldMoveJumpVelX = d[0] * k;
                        fldMoveJumpVelY = t * 8.166667f + (p[1] - o->obj->pos[1]) / t;
                        fldMoveJumpVelZ = d[2] * k;
                    } else {
                        fldMoveJumpVelX = d[0];
                        fldMoveJumpVelY = 0.0f;
                        fldMoveJumpVelZ = d[2];
                    }
                    o->unk30 = spd;
                    o->vel[0] = fldMoveJumpVelX;
                    o->vel[1] = fldMoveJumpVelY;
                    o->vel[2] = fldMoveJumpVelZ;
                    o->unk16 = 4;
                    if (!(n->flags & 0x20)) {
                        o->unk54 = FldDirXZToAngle(fldMoveJumpVelX, fldMoveJumpVelZ);
                    }
                } else if (n->state == 2) {
                    if (o->cnt++ < 30) {
                        o->cnt++;
                    } else {
                        n->state = 3;
                    }
                } else if (o->unk4C != 0) {
                    VUNIT(o->vel);
                    o->unk30 = 0.0f;
                    o->unk16 = 1;
                    o->cnt = 0;
                    if (!(n->flags & 0x20)) {
                        o->unk54 = FldDirXZToAngle(fldMoveJumpVelX, fldMoveJumpVelZ);
                    }
                    n->state = 2;
                }
            } else if (n->state == 2) {
                VUNIT(o->vel);
                o->unk30 = 0.0f;
                o->flags |= 0x80;
                n->state = 3;
            } else {
                if (n->state == 0) {
                    n->state++;
                }
                p = n->pos;
                spd = n->spd;
                VSUB(d, p, o->obj->pos);
                d[3] = 0.0f;
                len = fsqrt(VuVecDot(d, d));
                inv = 1.0f / len;
                if (spd < len) {
                    k = inv * spd;
                    o->vel[0] = d[0] * k;
                    o->vel[1] = d[1] * k;
                    o->vel[2] = d[2] * k;
                    o->unk30 = spd;
                    if ((o->flags & 4) && __builtin_fabsf(d[0]) < 0.5f && __builtin_fabsf(d[2]) < 0.5f) {
                        VUNIT(o->vel);
                        o->unk30 = len;
                        n->state = 2;
                    } else if (n->flags & 0x20) {
                        o->flags &= 0xFF7F;
                    }
                } else {
                    o->unk30 = len;
                    o->vel[0] = d[0];
                    if (o->flags & 4) {
                        o->vel[1] = 0.0f;
                    } else {
                        o->vel[1] = d[1];
                    }
                    o->vel[2] = d[2];
                    n->state = 2;
                }
            }
            break;
        case 3: {
            f32 w;
            p = n->pos;
            w = n->spd;
            if (n->flags & 0x20) {
                o->flags &= 0xFF7F;
            }
            if (w <= 0.0f) {
                VUNIT(o->vel);
                o->unk30 = 0.0f;
                o->flags |= 0x80;
                n->state = 3;
            } else {
                len = fsqrt(VuVecDot(d, d));
                o->unk30 = len;
                VuVecCopy(o->vel, p);
                n->spd -= 1.0f;
            }
            break;
        }
        }
        if (n->state == 3) {
            FldObjMoveCmdRemove(o, n);
            n = n->next;
            if (n == NULL) {
                break;
            }
            continue;
        }
        n = n->next;
        if (n == NULL) {
            break;
        }
        if (!(n->flags & 1)) {
            break;
        }
    }
}
