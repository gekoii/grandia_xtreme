#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n25.h"

void VuVecCopy(void *, void *);

s32 FldSpriteSpawn(u8 kind, void *pos, u8 arg2, u8 flags, u8 arg4) {
    s32 id;
    PtclFldSprite *s;

    id = FldSpriteFindFree();
    if (id == -1) {
        return -1;
    }
    s = &ptclFldSlots[id];
    s->unk26 = arg4;
    s->kind = kind;
    s->used = 1;
    s->alpha = 0x80;
    VuVecCopy(s->pos, pos);
    s->repeat = arg2;
    s->flags = flags;
    if (kind == 6) {
        s->size = VuRandInt(4) + 2;
        s->unk28 = VuRandInt(4) + 2;
    } else if (kind == 7 || kind == 8) {
        s->size = VuRandInt(3) + 3;
        s->unk28 = VuRandInt(3) + 3;
    } else {
        s->size = VuRandInt(12) + 6;
        s->unk28 = VuRandInt(12) + 6;
    }
    if (flags & 0x80) {
        if (VuRandInt(2) == 0) {
            s->flags |= 1;
        }
        if (VuRandInt(2) == 0) {
            s->flags |= 2;
        }
    } else {
        s->flags = flags;
    }
    s->vx = VuRandInt(6) - 3;
    s->vy = VuRandInt(10) - 5;
    s->vz = VuRandInt(6) - 3;
    FldSpriteSelectAnim(s->kind);
    GphAnimStateInit(s);
    fldSpriteWork->counts[s->kind]++;
    return id;
}
