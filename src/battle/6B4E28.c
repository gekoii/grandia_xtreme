#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSetColor(s32 idx, f32 x, f32 y, f32 z, f32 w) {
    EnvLight *l;
    GphLight *p;

    if (idx >= 3) {
        l = &envLight;
        GphLightListSet(l->lists[0]);
        p = GphLightGet(idx);
        p->color[0] = x;
        p->color[1] = y;
        p->color[2] = z;
        p->color[3] = w;
        GphLightListSet(l->lists[1]);
        p = GphLightGet(idx);
        p->color[0] = x;
        p->color[1] = y;
        p->color[2] = z;
        p->color[3] = w;
        GphLightListSet(l->lists[2]);
        p = GphLightGet(idx);
        p->color[0] = x;
        p->color[1] = y;
        p->color[2] = z;
        p->color[3] = w;
    }
}
