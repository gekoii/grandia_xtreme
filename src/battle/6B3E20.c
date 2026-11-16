/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/env.h"
#include "battle/misc.h"
#include "sys.h"

void GphShadowCalcMtx(FVECTOR *, FVECTOR *);

#define SET_AMB(src)                \
    p = GphLightGet(0);           \
    p->color[0] = (src)[0];       \
    p->color[1] = (src)[1];       \
    p->color[2] = (src)[2];       \
    p->color[3] = (src)[3];

#define SET_LIGHT(n, base, g, k)                \
    p = GphLightGet(n);                       \
    p->color[0] = (base)->color[g][0][0];       \
    p->color[1] = (base)->color[g][0][1];       \
    p->color[2] = (base)->color[g][0][2];       \
    p->color[3] = (base)->color[g][0][3];       \
    p->intensity = l->intensity[g][k];                    \
    p->dir[0] = (base)->dir[g][0][0];       \
    p->dir[1] = (base)->dir[g][0][1];       \
    p->dir[2] = (base)->dir[g][0][2];       \
    p->dir[3] = 0.0f;

void EnvLightInit(void) {
    EnvLight *l = &envLight;
    EnvLight *l2 = (EnvLight *)&l->amb[1];
    GphLight *p;
    EnvLight *m;
    FVECTOR *c;
    FVECTOR *c2;
    EnvParam *e;
    s32 x;
    s32 i;
    s32 n;
    s32 *q;
    f32 t;
    f32 u;
    f32 y;
    f32 w;

    x = MemAlloc16C(GphLightListSize(16));
    l->lists[0] = x;
    GphLightListInit(16, x);
    x = MemAlloc16C(GphLightListSize(16));
    l->lists[1] = x;
    GphLightListInit(16, x);
    x = MemAlloc16C(GphLightListSize(16));
    l->lists[2] = x;
    GphLightListInit(16, x);
    q = l->lightFlags;
    for (n = 15; n >= 0; n--) {
        *q++ = 0;
    }
    envDim.cur = 1.0f;
    envParamIndex = -1;
    envDim.step = 0;
    envDim.mask = 0;
    m = &envLight;
    c = envShadowMtx;
    EnvLightCalc();
    EnvLightAlloc(0x430000);

    GphLightListSet(m->lists[0]);
    SET_AMB(l->amb[0]);
    GphLightListSet(m->lists[1]);
    SET_AMB(l->amb[0]);
    GphLightListSet(m->lists[2]);
    SET_AMB(l->amb[1]);

    EnvLightAlloc(0x430000);
    EnvLightAlloc(0x430000);

    GphLightListSet(m->lists[0]);
    SET_LIGHT(1, l, 0, 0);
    SET_LIGHT(2, l2, 0, 1);
    GphLightListSet(m->lists[1]);
    SET_LIGHT(1, l, 0, 0);
    SET_LIGHT(2, l2, 0, 1);
    GphLightListSet(m->lists[2]);
    SET_LIGHT(1, l, 1, 0);
    SET_LIGHT(2, l2, 1, 1);
    envShadowLightIdx = 0;

    envShadowLightBlend = 0;
    GphShadowCalcMtx(c, &m->dir[0][0]);
    t = m->dir[0][0][0];
    if (m->dir[0][0][1] < 0.0f) {
        u = t / -m->dir[0][0][1];
    } else {
        u = t / m->dir[0][0][1];
    }
    c[1][0] = u;
    c2 = envShadowMtx;
    w = envLight.dir[0][0][2];
    if (envLight.dir[0][0][1] < 0.0f) {
        y = w / -envLight.dir[0][0][1];
    } else {
        y = w / envLight.dir[0][0][1];
    }
    c2[1][2] = y;
    envShadowMtx[3][1] = 0.9f;
    e = EnvParamGet(btlHandoff.mapNo);
    envLight.shadowColor[0] = e->shadowColor[0];
    envLight.shadowColor[1] = e->shadowColor[1];
    envLight.shadowColor[2] = e->shadowColor[2];
    envLight.shadowColor[3] = e->shadowColor[3];
    for (i = 0; i < 16; i++) {
        envShadowPreCb[i] = envShadowDrawCb[i] = 0;
    }
    if (btlEncounterFlags == 0x1A) {
        EnvDimStart(NULL, 0, 0.1f);
    }
}
