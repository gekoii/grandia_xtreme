/* as: ee-gcc2.9-991111 */
#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

#define APPLY_MATRIX(out, m, in) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(out), "r"(m), "r"(in) : "memory")

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    u8 pad4C[0x14];
    FVECTOR *unk60;
    u8 pad64[4];
    u32 *unk68;
} Obj;

s32 GphProjTexCalcUv(Obj *o, u32 *pkt, f32 (*m)[4]) {
    FVECTOR v;
    s32 n;
    s32 cnt;
    f32 *out;
    FVECTOR *src;
    f32 x, y, z;

    n = o->unk48;
    pkt[0] = 0x20000000;
    pkt[1] = 0x10101010;
    pkt[3] = (n << 16) | 0x74000080;
    cnt = 0;
    out = (f32 *)(pkt + 4);
    src = o->unk60;
    pkt[2] = 0;
    for (; n > 0; n--) {
        copyq(v, src);
        v[3] = 1.0f;
        APPLY_MATRIX(v, m, v);
        x = v[0];
        y = v[1];
        z = v[2];
        cnt++;
        if (!(__builtin_fabsf(x) <= 1.0f) && !(__builtin_fabsf(y) <= 1.0f) && !(__builtin_fabsf(z) <= 1.0f)) {
            do { cnt--; } while (0); /* do-while(0): keeps cnt-- a separate block (branch layout) */
        }
        *out++ = x;
        *out++ = y;
        src++;
    }
    o->unk68 = pkt;
    return cnt;
}
