#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvLightSetPos(s32 idx, f32 x, f32 y, f32 z) {
    EnvLight *l;
    GphLight *p;

    if (idx >= 3) {
        l = &envLight;
        GphLightListSet(l->lists[0]);
        p = GphLightGet(idx);
        p->dir[0] = x;
        p->dir[1] = y;
        p->dir[2] = z;
        p->dir[3] = 1.0f;
        GphLightListSet(l->lists[1]);
        p = GphLightGet(idx);
        p->dir[0] = x;
        p->dir[1] = y;
        p->dir[2] = z;
        p->dir[3] = 1.0f;
        GphLightListSet(l->lists[2]);
        p = GphLightGet(idx);
        p->dir[0] = x;
        p->dir[1] = y;
        p->dir[2] = z;
        p->dir[3] = 1.0f;
    }
}
