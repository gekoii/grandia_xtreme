/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "vu.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void VuCardinalInterp(FVECTOR *, FVECTOR *, f32 *, f32);

void WmapPathEval(FVECTOR *out, FVECTOR *k, s32 n, f32 t) {
    FVECTOR p[4];
    f32 tension;
    s32 i;
    s32 off;

    tension = 0.5f;
    if (k[0][3] == t) {
        copyq(out, k[0]);
        return;
    }
    off = n * 16;
    if (t <= k[1][3]) {
        copyq(p[0], k[0]);
        copyq(p[1], k[0]);
        copyq(p[2], k[1]);
        copyq(p[3], k[2]);
        VuCardinalInterp(out, p, &tension, (t - k[0][3]) / (k[1][3] - k[0][3]));
        return;
    }
    if (*(f32 *)(off + (u32)k - 20) <= t) {
        {
            s32 c, d, e;
            c = off - 48;
            copyq(p[0], (u8 *)k + c);
            d = off - 32;
            copyq(p[1], (u8 *)k + d);
            e = off - 16;
            copyq(p[2], (u8 *)k + e);
            copyq(p[3], (u8 *)k + e);
        }
        VuCardinalInterp(out, p, &tension, (t - *(f32 *)(off + (u32)k - 20)) / (*(f32 *)(off + (u32)k - 4) - *(f32 *)(off + (u32)k - 20)));
        return;
    }
    for (i = 1; i < n - 2; i++) {
        if (t < k[i + 1][3]) {
            s32 b = i * 16;
            s32 c, d, e;
            c = b - 16;
            copyq(p[0], (u8 *)k + c);
            copyq(p[1], k[i]);
            d = b + 16;
            copyq(p[2], (u8 *)k + d);
            e = b + 32;
            copyq(p[3], (u8 *)k + e);
            VuCardinalInterp(out, p, &tension, (t - k[i][3]) / (k[i + 1][3] - k[i][3]));
            return;
        }
    }
}
