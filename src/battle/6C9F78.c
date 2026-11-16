/* as: ee-gcc2.96-hazard */
/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"
#include "party.h"
#include "battle/camera.h"
#include "protos/battle.h"

extern s32 btlVictoryMotionEnd;

f32 VuSinAngle(s32);
void VuSinCosAngle(f32 *, f32 *, s32);
void VuVecCopy(VuVec3 *, VuVec3 *);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlCamVictoryCb(BtlCamCtrl *cam, BtlCamParam *ctx) {
    VuVec3 v;
    f32 sc[2];
    BtlUnit *u;
    f32 *p;
    f32 h0, one, a2;
    f32 k, s, t, h, base, a, b, c, w;

    switch (ctx->phase) {
    case 0:
        base = (rand() >> 15) * (1.0f / 65536.0f) * 180.0f + 220.0f;
        p = &btlVictoryCamDist;
        h = (rand() >> 15) * (1.0f / 65536.0f);
        h = base + (h < 0.2f ? 200.0f : 0.0f);
        *p = h;
        ctx->phase++;
    case 1:
        k = 1.0f;
        VuVecCopy(&v, &ctx->unitPtr->pos);
        VuSinCosAngle(&sc[0], &sc[1], ctx->unitPtr->rot[1]);
        v.x += sc[0] * 10.0f;
        v.z += sc[1] * 10.0f;
        if (ctx->unitPtr->frame < (f32)btlVictoryMotionEnd) {
            k = ctx->unitPtr->frame / (f32)btlVictoryMotionEnd;
        }
        s = VuSinAngle((s32)(k * 16384.0f));
        t = 1.0f - s;
        cam->to.rot[0] = (f32)(s32)(17408.0f - fsqrt(v.x * v.x + v.z * v.z) * 0.002f * 4096.0f) * s + (f32)cam->from.rot[0] * t;
        if (cam->from.rot[2] <= 0x7FFF) {
            cam->to.rot[2] = s * 0.0f + (f32)cam->from.rot[2] * t;
        } else {
            cam->to.rot[2] = s * 65536.0f + (f32)cam->from.rot[2] * t;
        }
        t = t * (t * t);
        s = 1.0f - t;
        u = ctx->unitPtr;
        cam->to.target[0] = v.x * s + cam->from.target[0] * t;
        if (u->params->id == 0x1008) {
            cam->to.target[1] = u->params->colH * 0.4f * s + cam->from.target[1] * t;
        } else {
            cam->to.target[1] = u->params->colH * 0.65f * s + cam->from.target[1] * t;
        }
        cam->to.target[2] = v.z * s + cam->from.target[2] * t;
        a = VuCosAngle((s32)(k * 16384.0f));
        w = 1.0f - a;
        cam->to.dist = btlVictoryCamDist * w + cam->from.dist * a;
        a = (VuCosAngle((s32)(k * 24576.0f)) + VuCosAngle(0x2000)) / (VuCosAngle(0x2000) + 1.0f);
        w = 1.0f - a;
        if (btlVictoryCamYaw & 1) {
            cam->to.rot[1] = (f32)cam->from.rot[1] + (f32)((btlVictoryCamYaw - cam->from.rot[1]) & 0xFFFF) * w;
        } else {
            cam->to.rot[1] = (f32)cam->from.rot[1] - (f32)((cam->from.rot[1] - btlVictoryCamYaw) & 0xFFFF) * w;
        }
        if (k >= 1.0f) {
            ctx->phase++;
        }
        break;
    default:
        if (btlVictoryCamYaw & 1) {
            cam->to.rot[1] += 8;
        } else {
            cam->to.rot[1] -= 8;
        }
        break;
    }
}
