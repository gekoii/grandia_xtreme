/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    FVECTOR m[4];
} View;

u32 fptoui(f32);

void GphLineVertsProject(u32 n, u64 *out, FVECTOR *in)
{
    FVECTOR v;
    View *view;
    f32 proj;
    f32 sx, sy, sz, ox, oy, oz;
    f32 k, x, y, z;
    u32 i;
    u64 pk;

    proj = (*(View **)0x700000EC)->m[1][2];
    view = (View *)VuMtxStackGetTop();
    sx = *(f32 *)0x70000060;
    sy = *(f32 *)0x70000064;
    sz = *(f32 *)0x70000068;
    ox = *(f32 *)0x70000070;
    oy = *(f32 *)0x70000074;
    oz = *(f32 *)0x70000078;
    for (i = 0; i < n; i++) {
        __asm__ volatile(
            "lqc2 $vf1, 0(%2)\n\t"
            "lqc2 $vf4, 0x0(%1)\n\t"
            "lqc2 $vf5, 0x10(%1)\n\t"
            "lqc2 $vf6, 0x20(%1)\n\t"
            "lqc2 $vf7, 0x30(%1)\n\t"
            "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
            "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
            "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
            "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(view), "r"(in) : "memory");
        if (v[2] < 0.01f) {
            v[2] = 1.0f;
        }
        k = proj / v[2];
        x = ox + v[0] * k * sx;
        y = oy + v[1] * k * sy;
        z = (oz + k * sz) * 0.0625f;
        if (x > 65520.0f) {
            x = 65520.0f;
        }
        if (x < 0.0f) {
            x = 0.0f;
        }
        if (y > 65520.0f) {
            y = 65520.0f;
        }
        if (y < 0.0f) {
            y = 0.0f;
        }
        pk = fptoui(x);
        pk |= (u64)fptoui(y) << 16;
        pk |= (u64)fptoui(z) << 32;
        *out = pk;
        out++;
        in++;
    }
}
