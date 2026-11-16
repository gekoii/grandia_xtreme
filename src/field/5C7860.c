/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

typedef struct {
    f32 *x;
    f32 *y;
    f32 *z;
    f32 *e;
} SegIn;

typedef struct {
    f32 *x;
    f32 *y;
    f32 *z;
} SegOut;

s32 MapPathLinearResample(SegIn *in, SegOut *out, s32 n) {
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    f32 px;
    f32 py;
    f32 pz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 st;

    j = 0;
    px = in->x[0];
    py = in->y[0];
    pz = in->z[0];
    for (i = 0; i < n - 1; i++) {
        st = in->e[i];
        out->x[j] = px;
        out->y[j] = py;
        out->z[j] = pz;
        j++;
        if (j >= 0x220) {
            j--;
            break;
        }
        dx = in->x[i + 1] - px;
        dy = in->y[i + 1] - py;
        dz = in->z[i + 1] - pz;
        d = fsqrt(dx * dx + dy * dy + dz * dz) / st;
        if (d < 1.0f) {
            d = 1.0f;
        }
        dx = dx / d;
        dy = dy / d;
        dz = dz / d;
        if (d >= 1.0f) {
            cnt = (s32)(d + 0.5f) + 1;
            for (k = 1; k < cnt; k++) {
                out->x[j] = px + dx * k;
                out->y[j] = py + dy * k;
                out->z[j] = pz + dz * k;
                j++;
                if (j >= 0x220) {
                    break;
                }
            }
        } else {
            out->x[j] = in->x[i + 1];
            out->y[j] = in->y[i + 1];
            out->z[j] = in->z[i + 1];
            j++;
        }
        j--;
        px = out->x[j];
        py = out->y[j];
        pz = out->z[j];
    }
    out->x[j] = in->x[n - 1];
    out->y[j] = in->y[n - 1];
    out->z[j] = in->z[n - 1];
    j++;
    return j * 3;
}
