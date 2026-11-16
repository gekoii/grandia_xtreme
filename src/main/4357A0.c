/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"

typedef int s128 __attribute__((mode(TI)));

void GphMorphPositionsLerp(GphPart *n, s128 *a, s128 *b, f32 t);

void GphMorphPositions(GphPart *n, GphMotKeys *t, f32 time) {
    s32 i;
    u16 cnt;
    s32 total = 0;
    GphGaObject *j = n->gaObjs;
    u16 *kp;
    s128 *data;
    s32 k;
    f32 f;
    s32 k1;

    cnt = n->gaObjCount;
    for (i = 0; i < cnt; i++) {
        total += j->vtxCount;
        j++;
    }
    data = t->values;
    k = t->count;
    if (k == 1) {
        GphMorphPositionsCopy(n, data);
        return;
    }
    k = (s32)time;
    kp = t->frames;
    k = t->frameIndex[k];
    f = kp[k];
    if (f == time) {
        GphMorphPositionsCopy(n, data + k * total);
        return;
    }
    k1 = k + 1;
    {
    f32 r = (time - f) / ((f32)kp[k1] - f);
    GphMorphPositionsLerp(n, data + k * total, data + k1 * total, r);
    }
}
