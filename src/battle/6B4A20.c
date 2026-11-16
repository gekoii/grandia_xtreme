#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/env.h"


s32 EnvLightAlloc(s32 flags) {
    s32 *p = envLightFlags;
    s32 i;
    EnvLight *l;
    GphLight *a;
    GphLight *b;
    GphLight *c;
    s32 v;

    for (i = 0; i < 16; i++, p++) {
        if (!(*p & 0x800000)) {
            break;
        }
    }
    if (!(i < 16)) {
        return -1;
    }
    *p = flags | 0x800000;
    l = &envLight;
    GphLightListSet(l->lists[0]);
    a = GphLightGet(i);
    GphLightListSet(l->lists[1]);
    b = GphLightGet(i);
    GphLightListSet(l->lists[2]);
    c = GphLightGet(i);
    GphLightInit(a);
    GphLightInit(b);
    GphLightInit(c);
    v = ((flags >> 22) ^ 1) & 1;
    a->type = v;
    b->type = v;
    c->type = v;
    if (flags & 0x10000) {
        a->enable = 1;
        b->enable = 1;
    }
    if (flags & 0x20000) {
        c->enable = 1;
    }
    EnvLightSetIntensity(i, 1.0f);
    return i;
}
