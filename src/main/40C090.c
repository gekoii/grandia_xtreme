/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad[0x60];
    FVECTOR pos;
    FVECTOR dir;
} Cam;

typedef struct {
    Cam *cam;
} Spr;

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void VuVecNormalize(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void VuVecCopy(void *, void *);
void VuProjectVerts(f32 *, s32, f32 *, s32);

s32 UtilWorldToScreen(f32 *out, f32 *pos)
{
    FVECTOR d;
    FVECTOR f;
    FVECTOR t;
    FVECTOR u; /* unused: sizes the frame (16 more bytes) */
    Spr *s = (Spr *)0x70002700;

    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyzw $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(d), "r"(pos), "r"(s->cam->pos) : "memory");
    copyq(f, s->cam->dir);
    VuVecNormalize(d, d);
    VuVecNormalize(f, f);
    if (!(VuVecDot(f, d) > 0.5f)) {
        out[1] = 2500.0f;
        out[0] = 2500.0f;
        return 0;
    }
    VuVecCopy(t, pos);
    VuProjectVerts(t, VuMtxStackGetTop(), t, 1);
    VuVecCopy(out, t);
    return 1;
}
