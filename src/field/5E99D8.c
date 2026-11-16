/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n17.h"



void FldLensFlareDrawGhost(s32 id, GphBatchQuad *q, f32 alpha, f32 t, f32 size) {
    VuVec4 v;
    s32 a;

    a = alpha * 128.0f;
    q->v[3].a = a;
    q->v[2].a = a;
    q->v[1].a = a;
    q->v[0].a = a;
    v.x = camField[0].lookAt[0] + (fldSunPos[0] - camField[0].lookAt[0]) * t;
    v.y = camField[0].lookAt[1] + (fldSunPos[1] - camField[0].lookAt[1]) * t;
    v.z = camField[0].lookAt[2] + (fldSunPos[2] - camField[0].lookAt[2]) * t;
    if (UtilWorldToScreen(&v, &v) == 0) {
        return;
    }
    if (v.x <= -size || size + 640.0f <= v.x || v.y <= -size || size + 448.0f <= v.y) {
        return;
    }
    q->v[1].x = v.x - size;
    q->v[3].x = v.x + size;
    q->v[2].y = v.y - size;
    q->v[3].y = v.y + size;
    q->v[2].x = v.x + size;
    q->v[0].x = v.x - size;
    q->v[0].y = v.y - size;
    q->v[1].y = v.y + size;
    GphTexBind(id - 0x40);
    GphBatchDrawQuads(q);
}
