/* Port-only C for GphGaObjEnvMapCalcUv (0x0043BD80), which the decompilation still has in assembly. */
/*
 * VU0 registers: the PS2 leaves the matrix rows in $vf12-$vf14; the C
 * callers (src/main/43BAE0.c, 43BC10.c) don't read them, so the port
 * doesn't write grx_vf.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

void GphGaObjEnvMapCalcUv(GphGaObject *obj, f32 *mtx);

/*
 * Sphere-map UVs: each 4.12 normal (+0x64, after its 16-byte header) is
 * rotated by mtx's 3x3; u = (1 + n.x), v = (1 - n.y) in 4.12 >> 3, as s16
 * pairs after the 16-byte header of obj->envUv. Four normals per step (the
 * count is rounded up to 4), then the padding entries are cleared as the
 * asm does it (one 32-bit or one 64-bit store: 3 entries of padding get
 * only 2 cleared).
 */
void GphGaObjEnvMapCalcUv(GphGaObject *obj, f32 *mtx) {
    u8 *uv = obj->envUv;
    s32 n = obj->vtxCount;
    s16 *src = (s16 *)((u8 *)obj->normals + 0x10);
    s16 *dst = (s16 *)(uv + 0x10);
    s32 groups = (n + 3) >> 2;
    s32 pad;

    for (; groups > 0; groups--) {
        s32 k;

        for (k = 0; k < 4; k++) {
            f32 v[4];
            f32 r[3];
            s32 j;

            for (j = 0; j < 4; j++) {
                v[j] = VuItof(src[k * 4 + j], 12);
            }
            for (j = 0; j < 3; j++) {
                r[j] = mtx[j] * v[0] + mtx[4 + j] * v[1] + mtx[8 + j] * v[2];
            }
            dst[k * 2 + 0] = (s16)(VuFtoi(1.0f + r[0], 12) >> 3);
            dst[k * 2 + 1] = (s16)(VuFtoi(1.0f + (0.0f - r[1]), 12) >> 3);
        }
        src += 16;
        dst += 8;
    }

    pad = n & 3;
    if (pad != 0) {
        u8 *p = uv + n * 4 + 0x10;

        pad = 4 - pad;
        if (pad == 1) {
            *(u32 *)p = 0;
        } else {
            /* 2, or 3 (sd then sw to the same address) */
            *(u32 *)p = 0;
            *(u32 *)(p + 4) = 0;
        }
    }
}
