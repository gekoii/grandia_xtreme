/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "battle/camera.h"
#include "battle/n28.h"

f32 BtlChrGetHeight(BtlUnit *);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlSpecialIntroCamGroup(BtlCamCtrl *c, BtlCamParam *x) {
    BtlUnit *u;
    BtlUnit *o;
    BtlUnit *q;
    s32 i;
    s32 j;
    f32 minx, maxx, minz, maxz, maxh, h;
    f32 d, t, px, pz;

    if (x->phase == 0) {
        u = x->unitPtr;
        maxx = u->pos[0];
        maxz = u->pos[2];
        minx = maxx;
        maxh = BtlChrGetHeight(u);
        minz = maxz;
        for (i = 0; i < 3; i++) {
            if (u->comboPartners[i] < 0) {
                break;
            }
            o = &btlUnits[u->comboPartners[i]];
            if (o->pos[0] < minx) {
                minx = o->pos[0];
            }
            if (maxx < o->pos[0]) {
                maxx = o->pos[0];
            }
            if (o->pos[2] < minz) {
                minz = o->pos[2];
            }
            if (maxz < o->pos[2]) {
                maxz = o->pos[2];
            }
            h = BtlChrGetHeight(o);
            if (maxh < h) {
                maxh = h;
            }
        }
        c->to.target[0] = (minx + maxx) * 0.5f;
        c->to.target[2] = (minz + maxz) * 0.5f;
        c->to.target[1] = maxh * 0.5f + 10.0f;
        if (btlSpecialIntro.count != 2) {
            c->to.target[2] -= 100.0f;
        }
        px = u->pos[0] - c->to.target[0];
        pz = u->pos[2] - c->to.target[2];
        d = px * px + pz * pz;
        for (j = 0; j < 3; j++) {
            if (u->comboPartners[j] < 0) {
                break;
            }
            q = &btlUnits[u->comboPartners[j]];
            t = (q->pos[0] - c->to.target[0]) * (q->pos[0] - c->to.target[0]) + (q->pos[2] - c->to.target[2]) * (q->pos[2] - c->to.target[2]);
            if (d < t) {
                d = t;
            }
        }
        c->to.dist = fsqrt(d) * 1.6f + 250.0f;
        c->to.rot[0] = btlSpecialIntro.count == 2 ? 0x3E00 : 0x3000;
        c->to.rot[1] = 0;
        x->phase++;
    }
}
