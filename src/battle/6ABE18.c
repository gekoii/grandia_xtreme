/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void PtclBurstUpdateDebris(PtclBurst *arg0) {
    PtclDebris *e;
    PtclDebris *base;
    GphModel *o;
    s32 i;

    e = arg0->parts;
    base = e;
    if (arg0->active > 0) {
        if (arg0->unk8 != btlFrameCount) {
            PtclBurstFree(arg0);
        }
        return;
    }
    i = 7;
    do {
        if (!((e->life -= 0.03f) < 0.0f)) {
            e->vel[1] -= 0.5f;
            o = e->model;
            o->rot[0] += e->rotVel[0];
            o->pos[0] += e->vel[0];
            o->rot[1] += e->rotVel[1];
            o->pos[1] += e->vel[1];
            o->rot[2] += e->rotVel[2];
            o->pos[2] += e->vel[2];
        }
        i--;
        e++;
    } while (i >= 0);
    e = base;
    for (i = 0; i < 8; i++, e++) {
        if (e->life > 0.0f) {
            break;
        }
    }
    if (i >= 8) {
        arg0->active = 1;
        arg0->unk8 = btlFrameCount;
    }
}
