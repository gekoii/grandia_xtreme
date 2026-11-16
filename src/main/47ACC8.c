/* cflags: -fno-builtin */
/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef union {
    f32 value;
    u32 word;
} ieee_float_shape_type;

#define GET_FLOAT_WORD(i, d)          \
    do {                              \
        ieee_float_shape_type gf_u;   \
        gf_u.value = (d);             \
        (i) = gf_u.word;              \
    } while (0)

extern f32 atanhi[];
extern f32 atanlo[];
extern f32 aT[];

f32 atanf(f32 x) {
    f32 w, s1, s2, z;
    s32 ix, hx, id;

    GET_FLOAT_WORD(hx, x);
    ix = hx & 0x7fffffff;
    if (ix >= 0x50800000) {
        if (ix > 0x7f800000)
            return x + x;
        if (hx > 0)
            return atanhi[3] + atanlo[3];
        else
            return -atanhi[3] - atanlo[3];
    }
    if (ix < 0x3ee00000) {
        if (ix < 0x31000000) {
            if (1.0e30f + x > 1.0f)
                return x;
        }
        id = -1;
    } else {
        x = fabsf(x);
        if (ix < 0x3f980000) {
            if (ix < 0x3f300000) {
                id = 0;
                x = (2.0f * x - 1.0f) / (2.0f + x);
            } else {
                id = 1;
                x = (x - 1.0f) / (x + 1.0f);
            }
        } else {
            if (ix < 0x401c0000) {
                id = 2;
                x = (x - 1.5f) / (1.0f + 1.5f * x);
            } else {
                id = 3;
                x = -1.0f / x;
            }
        }
    }
    z = x * x;
    w = z * z;
    s1 = z * (aT[0] + w * (aT[2] + w * (aT[4] + w * (aT[6] + w * (aT[8] + w * aT[10])))));
    s2 = w * (aT[1] + w * (aT[3] + w * (aT[5] + w * (aT[7] + w * aT[9]))));
    if (id < 0)
        return x - x * (s1 + s2);
    else {
        z = atanhi[id] - ((x * (s1 + s2) - atanlo[id]) - x);
        return (hx < 0) ? -z : z;
    }
}
