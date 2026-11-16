/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"

void GphMorphNormalsLerp(GphPart *n, s64 *a, s64 *b, f32 t);

void GphMorphNormals(GphPart *n, GphMotKeys *t, f32 time) {
    s32 i;
    u16 cnt;
    s32 total = 0;
    GphGaObject *j = n->gaObjs;
    u16 *kp;
    s64 *data;
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
        GphMorphNormalsCopy(n, data);
        return;
    }
    k = (s32)time;
    kp = t->frames;
    k = t->frameIndex[k];
    f = kp[k];
    if (f == time) {
        GphMorphNormalsCopy(n, data + k * total);
        return;
    }
    k1 = k + 1;
    {
    f32 r = (time - f) / ((f32)kp[k1] - f);
    GphMorphNormalsLerp(n, data + k * total, data + k1 * total, r);
    }
}
