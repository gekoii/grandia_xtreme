#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"

void BtlVecRandomDir(f32 *);

#define RND() ((f32)(rand() >> 15) * (1.0f / 65536.0f))

/* out = in * k (xyz) */
#define VSCALE(out, in, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(out), "r"(in), "r"(k) : "memory")

/* Same, with the store as its own statement (it lands in a jal delay slot). */
#define VSCALE2(out, in, k) { \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "qmtc2.ni %1, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x" \
        : : "r"(in), "r"(k) : "memory"); \
    __asm__ volatile("sqc2 $vf1, 0(%0)" : : "r"(out) : "memory"); \
}

/* Same, through pointer temporaries that are set before k is evaluated. */
#define RSCALE(out, in, k) { \
    f32 *_o = (out); \
    f32 *_i = (in); \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%0)\n\t" \
        "qmtc2.ni %1, $vf2\n\t" \
        "vmulx.xyz $vf1, $vf1, $vf2x" \
        : : "r"(_i), "r"(k) : "memory"); \
    __asm__ volatile("sqc2 $vf1, 0(%0)" : : "r"(_o) : "memory"); \
}

/* out = a + b (xyz) */
#define VADD(out, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(out), "r"(a), "r"(b) : "memory")

void PtclBtlSprEmit(PtclBtlSpr *p, f32 *base, s32 type, s8 mode, f32 scale)
{
    f32 v[4] __attribute__((aligned(16)));
    GphAnimSet **s;
    GphAnimSet *tex;

    s = ptclBurstTypeTable[type].animSet;
    tex = NULL;
    if (s != NULL) {
        tex = *s;
    }
    GphAnimSetSelect(tex);
    BtlVecRandomDir(p->pos);
    p->flags = (rand() >> 15) & 3;
    p->life = ptclBurstTypeTable[type].unk1 + 1;
    switch (type) {
    case 1: {
        f32 k;
        if (mode == 0) {
            k = RND() * 4.0f + 2.0f;
        } else {
            k = RND() * 2.0f + 0.5f;
        }
        k = k * scale;
        BtlVecRandomDirInRange(v, 0, 0x1300, 0, 0xFFFF);
        p->vel[0] = v[0] * k;
        p->vel[1] = v[1] * k;
        p->vel[2] = v[2] * k;
        p->accel[0] = p->accel[1] = p->accel[2] = 0.0f;
        if (mode == 0) {
            VSCALE(p->pos, p->pos, 50.0f);
            p->pos[1] -= 30.0f;
            p->scale = RND() * 13.5f + 4.0f;
        } else {
            p->scale = RND() * 12.0f + 3.0f;
        }
        p->flags &= 1;
        break;
    }
    case 2:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 4.0f, scale * 6.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f);
            p->scale = RND() * 5.0f + 15.0f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
            p->scale = RND() * 15.0f + 10.0f;
        }
        break;
    case 3:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 4.0f, scale * 6.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f);
            p->scale = RND() * 10.0f + 2.5f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
            p->scale = RND() * 10.0f + 4.0f;
        }
        break;
    case 4:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 6.0f, scale * 8.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f + 20.0f);
            p->scale = RND() * 10.0f + 10.0f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 2.0f, scale * 2.0f);
            p->scale = RND() * 20.0f + 5.0f;
        }
        break;
    case 5:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 3.0f, scale * 4.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f + 0.0f);
            p->scale = RND() * 5.0f + 7.5f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
            p->scale = RND() * 8.0f + 5.0f;
        }
        break;
    case 6:
        p->vel[0] = p->vel[1] = p->vel[2] = p->accel[0] = p->accel[1] = p->accel[2] = 0.0f;
        if (mode == 0) {
            RSCALE(p->pos, p->pos, RND() * 100.0f + 20.0f);
            p->scale = RND() * 10.0f + 10.0f;
        } else {
            p->scale = RND() * 15.0f + 20.0f;
        }
        break;
    case 7:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 8.0f, scale * 6.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f + 0.0f);
            p->scale = RND() * 7.5f + 2.5f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
            p->scale = RND() * 10.0f + 6.0f;
        }
        break;
    case 8:
        if (mode == 0) {
            PtclBtlSprSetSpeed(p, scale * 6.0f, scale * 8.0f);
            RSCALE(p->pos, p->pos, RND() * 50.0f + 20.0f);
            p->scale = RND() * 10.0f + 5.0f;
        } else {
            PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
            p->scale = RND() * 15.0f + 10.0f;
        }
        break;
    case 10:
        PtclBtlSprSetSpeed(p, scale * 0.5f, scale * 2.0f);
        p->scale = RND() * 10.0f + 6.0f;
        break;
    case 11:
    case 12:
        PtclBtlSprSetSpeed(p, scale * 6.0f, scale * 10.0f);
        p->scale = RND() * 10.0f + 20.0f;
        break;
    case 13:
    case 14:
    case 15:
        PtclBtlSprSetSpeed(p, scale * 0.3f, scale);
        p->scale = RND() * 10.0f + 6.0f;
        break;
    case 16: {
        f32 r;
        PtclBtlSprSetSpeed(p, scale * 0.3f, scale);
        r = RND();
        p->flags &= 1;
        p->scale = r * 15.0f + 10.0f;
        break;
    }
    default:
        PtclBtlSprSetSpeed(p, scale * 14.0f, scale * 2.0f);
        VSCALE2(p->pos, p->pos, 50.0f);
        p->scale = RND() * 5.0f + 2.5f;
        break;
    }
    p->scale *= scale;
    VSCALE(p->pos, p->pos, scale);
    VADD(p->pos, p->pos, base);
    GphAnimStateInit(&p->anim);
}
