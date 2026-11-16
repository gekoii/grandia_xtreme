/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "battle/env.h"
#include "protos/battle.h"

void EnvDimApply(void) {
    EnvLight *l = &envLight;
    GphLight *p;
    f32 zero = 0.0f;
    f32 k;
    f32 o;
    f32 v;

    GphLightListSet(l->lists[1]);
    v = envDim.cur + zero;
    if (v < zero) {
        v = zero;
    }
    k = 1.5f;
    p = GphLightGet(0);
    o = -0.5f;
    p->color[0] = l->amb[0][0] * v;
    p->color[1] = l->amb[0][1] * v;
    p->color[2] = l->amb[0][2] * v;
    p->color[3] = l->amb[0][3];
    p = GphLightGet(1);
    v = envDim.cur * k + o;
    if (v < zero) {
        v = zero;
    }
    p->intensity = l->intensity[0][0] * v;
    p = GphLightGet(2);
    v = envDim.cur * k + o;
    if (v < zero) {
        v = zero;
    }
    p->intensity = l->intensity[0][1] * v;
    GphLightListSet(l->lists[2]);
    v = envDim.cur + zero;
    if (v < zero) {
        v = zero;
    }
    p = GphLightGet(0);
    p->color[0] = l->amb[1][0] * v;
    p->color[1] = l->amb[1][1] * v;
    p->color[2] = l->amb[1][2] * v;
    p->color[3] = l->amb[1][3];
    p = GphLightGet(1);
    v = envDim.cur * k + o;
    if (v < zero) {
        v = zero;
    }
    p->intensity = l->intensity[1][0] * v;
    p = GphLightGet(2);
    v = envDim.cur * k + o;
    if (v < zero) {
        v = zero;
    }
    p->intensity = l->intensity[1][1] * v;
    envLightListSel = -1;
}
