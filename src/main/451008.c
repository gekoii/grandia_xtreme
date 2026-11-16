/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"

typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec;

typedef struct {
    u8 pad0[0x3F];
    u8 active;
} Part;

typedef struct {
    Part p[31];
    Vec base;
    u8 pad7D0[0x50];
    Vec org;
    Vec dst;
    Vec cur;
    Vec step;
    Vec rnd;
    u8 pad870[0x1294 - 0x870];
    f32 f1294;
    f32 f1298;
    u8 pad129C[8];
    f32 f12A4;
    u8 pad12A8[4];
    FMATRIX *mtx;
    u8 count2;
    u8 count;
    u8 pad12B2[8];
    u8 f12BA;
} Emitter;


#define RANDF() (((f32)rand() - (f32)rand()) * 2.3283064e-10f)

#define UNIT_MATRIX(m) __asm__ volatile( \
    "vmr32.xyzw $vf5, $vf0\n\t" \
    "vmr32.xyzw $vf6, $vf5\n\t" \
    "vmr32.xyzw $vf7, $vf6\n\t" \
    "sqc2 $vf0, 0x30(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf6, 0x10(%0)\n\t" \
    "sqc2 $vf7, 0x0(%0)" \
    : : "r"(m) : "memory")

#define TRANS(d, m, t) __asm__ volatile( \
    "lqc2 $vf1, 0x30(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "lqc2 $vf3, 0x0(%1)\n\t" \
    "lqc2 $vf4, 0x10(%1)\n\t" \
    "lqc2 $vf5, 0x20(%1)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf3, 0x0(%0)\n\t" \
    "sqc2 $vf4, 0x10(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf1, 0x30(%0)" \
    : : "r"(d), "r"(m), "r"(t) : "memory")

void PtclEmitterStart(Emitter *em)
{
    f32 v[4] __attribute__((aligned(16)));
    s32 i;

    if (em->count == 0 || em->count2 == 0) {
        return;
    }
    if (em->count > 31) {
        em->count = 31;
    }
    em->f12BA = 1;
    em->f12A4 = 0.0f;
    em->cur.x = em->org.x;
    em->cur.y = em->org.y;
    em->cur.z = em->org.z;
    em->cur.w = em->org.w;
    em->step.x = (em->dst.x - em->org.x) / em->f1294;
    em->step.y = (em->dst.y - em->org.y) / em->f1294;
    em->step.z = (em->dst.z - em->org.z) / em->f1294;
    em->step.w = (em->dst.w - em->org.w) / em->f1294;
    for (i = 0; i < em->count; i++) {
        ((void (*)(Emitter *, u8))PtclEmitterSpawn)(em, i); /* u8 index: masked as a byte */
        em->f12A4 += em->f1298 + em->f1298 * RANDF() * 0.5f;
    }
    em->f12A4 = 0.0f;
    if (em->count != 31) {
        for (i = em->count; i < 31; i++) {
            ((Part *)((i << 6) + (u32)em))->active = 0;
        }
    }
    for (i = 0; i < em->count2; i++) {
        v[0] = em->base.x + em->rnd.x * RANDF();
        v[1] = em->base.y + em->rnd.y * RANDF();
        v[2] = em->base.z + em->rnd.z * RANDF();
        v[3] = 1.0f;
        if (em->mtx[i] != NULL) {
            UNIT_MATRIX(em->mtx[i]);
            TRANS(em->mtx[i], em->mtx[i], v);
        }
    }
}
