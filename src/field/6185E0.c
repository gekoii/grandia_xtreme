/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n22.h"

typedef struct {
    f32 x, y, z;
} Vec3;

extern Vec3 fldPointLightNearestPos;

Vec3 *FldPointLightGetNearestPos(void) {
    f32 best;
    s32 idx;
    u32 i;
    f32 d;
    FldLightRec *sub;

    best = 99999.0f;
    idx = -1;
    if (fldPointLightCount == 0) {
        return NULL;
    }
    for (i = 0; i < fldPointLightCount; i++) {
        d = fldPointLightSort[i].dist;
        if (d < best) {
            best = d;
            idx = i;
        }
    }
    if (idx == -1 || best > 300.0f) {
        return NULL;
    }
    sub = &fldPointLights[idx].light;
    if (sub == NULL) {
        return NULL;
    }
    fldPointLightNearestPos.x = sub->pos[0];
    fldPointLightNearestPos.y = sub->pos[1];
    fldPointLightNearestPos.z = sub->pos[2];
    return &fldPointLightNearestPos;
}
