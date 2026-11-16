#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n25.h"


void FldSpriteUpdate(void) {
    s32 i;
    PtclFldSprite *s;

    for (i = 0; i < 0x40; i++) {
        s = &ptclFldSlots[i];
        if (s->used == 0) {
            continue;
        }
        FldSpriteSelectAnim(s->kind);
        GphAnimStep(s, 1.0f);
        if (s->kind == 6) {
            s->pos[1] += s->alpha * 0.0078125f * 2.0f;
            s->pos[0] += VuRandSigned(1.5f);
            s->pos[2] += VuRandSigned(1.5f);
        }
        if (s->kind == 7 || s->kind == 8) {
            s->pos[0] += s->vx;
            s->pos[1] += s->vy;
            s->pos[2] += s->vz;
            if (s->kind == 8) {
                s->vy--;
            }
        }
        s->alpha -= s->unk26;
        if (s->alpha == 0) {
            if (s->repeat != 0) {
                s->repeat--;
                GphAnimStateInit(s);
            } else {
                s->used = 0;
                if (fldSpriteWork->counts[s->kind] != 0) {
                    fldSpriteWork->counts[s->kind]--;
                }
            }
        }
    }
}
