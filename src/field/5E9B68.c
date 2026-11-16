/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n17.h"


static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void FldLensFlareDraw(void) {
    VuVec4 v;
    GphBatchQuad q[2];
    f32 d;
    f32 b;
    f32 dist;
    f32 z;

    if (fldLensFlareEnable == 0) {
        return;
    }
    z = 0.0f;
    if (fldLensFlareAlpha <= z) {
        return;
    }
    if (UtilWorldToScreen((f32 *)&v, fldSunPos) == 0) {
        return;
    }
    dist = fsqrt((v.x - 320.0f) * (v.x - 320.0f) + (v.y - 224.0f) * (v.y - 224.0f)) * 0.005f;
    if (v.x < z) {
        v.x = z - v.x;
    } else if (v.x > 640.0f) {
        v.x = v.x - 640.0f;
    } else {
        v.x = z;
    }
    if (v.y < 0.0f) {
        v.y = 0.0f - v.y;
    } else if (v.y > 448.0f) {
        v.y = v.y - 448.0f;
    } else {
        v.y = 0.0f;
    }
    if (v.y > v.x) {
        v.x = v.y;
    }
    d = v.x;
    if (d > 256.0f) {
        b = 0.0f;
    } else {
        b = (256.0f - d) * (1.0f / 256.0f) * fldLensFlareAlpha;
    }
    if (!(b > 0.0f)) {
        return;
    }
    q[0].type = 2;
    q[1].type = 0;
    q[0].v[0].r = 0xFF;
    q[0].v[0].g = 0xFF;
    q[0].v[0].b = 0xFF;
    q[0].v[0].a = 0x80;
    q[0].v[1].r = 0xFF;
    q[0].v[1].g = 0xFF;
    q[0].v[1].b = 0xFF;
    q[0].v[1].a = 0x80;
    q[0].v[2].r = 0xFF;
    q[0].v[2].g = 0xFF;
    q[0].v[2].b = 0xFF;
    q[0].v[2].a = 0x80;
    q[0].v[3].r = 0xFF;
    q[0].v[3].g = 0xFF;
    q[0].v[3].b = 0xFF;
    q[0].v[3].a = 0x80;
    q[0].v[0].u = 0;
    q[0].v[0].v = 0;
    q[0].v[1].u = 0;
    q[0].v[1].v = 0xFF;
    q[0].v[2].u = 0xFF;
    q[0].v[2].v = 0;
    q[0].v[3].u = 0xFF;
    q[0].v[3].v = 0xFF;
    FldLensFlareDrawGhost(0, q, b * 0.52f, 1.0f, 320.0f);
    q[0].v[0].u = 0;
    q[0].v[0].v = 0;
    q[0].v[1].u = 0;
    q[0].v[1].v = 0x7F;
    q[0].v[2].u = 0x7F;
    q[0].v[2].v = 0;
    q[0].v[3].u = 0x7F;
    q[0].v[3].v = 0x7F;
    FldLensFlareDrawGhost(1, q, b * 0.48f, dist * 0.25f, 32.0f);
    FldLensFlareDrawGhost(2, q, b * 0.44f, dist * 0.15f, 28.0f);
    FldLensFlareDrawGhost(3, q, b * 0.4f, dist * 0.075f, 12.0f);
    FldLensFlareDrawGhost(4, q, b * 0.36f, dist * 0.003f, 56.0f);
    FldLensFlareDrawGhost(5, q, b * 0.32f, dist * -0.002f, 24.0f);
    FldLensFlareDrawGhost(6, q, b * 0.28f, dist * -0.015f, 64.0f);
    FldLensFlareDrawGhost(7, q, b * 0.24f, dist * -0.025f, 112.0f);
}
