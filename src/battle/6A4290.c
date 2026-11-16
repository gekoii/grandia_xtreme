#include "common.h"
#include "protos/battle.h"
#include "vu.h"

#include "battle/n30.h"


#define VSCALE(v, s) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "qmtc2.ni %1, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(v), "r"(s) : "memory")

void FxDefeatBurstUpdate(BtlUnit *arg0) {
    FxDefeat *p = &fxDefeat[arg0->id];
    FxDefeatPart *e;
    VuVec4 v;
    VuVec4 d;
    f32 s;
    s32 i;
    s32 t;
    s8 alive;

    e = p->parts;
    if (e == NULL) {
        return;
    }
    for (p->unk4 += 3; p->unk4 >= 2; p->unk4 -= 2) {
        if (p->unk5 < 15) {
            FxDefeatPart *q = &e[p->unk5];
            t = rand();
            q->age = 0;
            q->life = (t >> 15) * (1.0f / 65536.0f) * 20.0f + 40.0f;
            q->sx = ((rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 15.0f) * 0.5f;
            q->sy = ((rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 15.0f) * 0.5f;
            BtlChrGetPartPos(&v, arg0, 1);
            BtlVecAddRandomOffset(&v, 0.0f, 100.0f);
            q->pos[0] = v.x;
            q->pos[1] = v.y;
            q->pos[2] = v.z;
            BtlVecRandomDirInRange(&d, 0, 0x700, 0, 0xFFFF);
            s = (rand() >> 15) * (1.0f / 65536.0f) * 5.0f + 0.01f;
            VSCALE(&d, s);
            e = p->parts;
            q->vel[0] = d.x;
            q->vel[1] = d.y;
            q->vel[2] = d.z;
            p->unk5++;
        }
    }
    alive = 0;
    i = 15;
    do {
        if (e->life > 0) {
            e->age++;
            if (e->age >= e->life) {
                e->life = 0;
            } else {
                alive++;
                e->pos[0] += e->vel[0];
                e->pos[1] += e->vel[1];
                e->pos[2] += e->vel[2];
            }
        }
        i--;
        e++;
    } while (i > 0);
    if (alive == 0) {
        MemFree(p->parts);
        p->parts = NULL;
    }
}
