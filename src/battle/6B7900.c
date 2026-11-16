/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/env.h"
#include "battle/n31.h"


void EnvFlareDrawSprite(s32 id, GphBatchQuad *q, f32 alpha, f32 t, f32 size) {
    VuVec4 v;
    s32 a;

    a = alpha * 128.0f;
    q->v[3].a = a;
    q->v[2].a = a;
    q->v[1].a = a;
    q->v[0].a = a;
    v.x = btlCamParam.target[0] + (envFlarePos[0] - btlCamParam.target[0]) * t;
    v.y = btlCamParam.target[1] + (envFlarePos[1] - btlCamParam.target[1]) * t;
    v.z = btlCamParam.target[2] + (envFlarePos[2] - btlCamParam.target[2]) * t;
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
    GphTexBind(id + 0x65);
    GphBatchDrawQuads(q);
}
