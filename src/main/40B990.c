#include "common.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x50];
    union { f32 f; s32 i; } x;
    f32 y;
    f32 z;
    u8 pad5C[0x90 - 0x5C];
    f32 rx;
    f32 ry;
    f32 rz;
    f32 dist;
    s32 unkA0;
} Cam;

typedef struct {
    u16 buttons;
    u8 pad2[0xC];
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 unk11;
    u8 pad12[4];
    u8 unk16;
    u8 unk17;
    u8 pad18[6];
    u8 rx;
    u8 ry;
    u8 lx;
    u8 ly;
} Pad;

#define APPLY_MATRIX(v0, m, v1) __asm__ volatile( \
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
    : : "r"(v0), "r"(m), "r"(v1) : "memory")

void VuMtxSetRotY(FVECTOR *, s32);

void CamDebugPadControl(Cam *c, Pad *p)
{
    FMATRIX m;
    FVECTOR v;

    c->unkA0 = 0x39;
    if (p->lx < 0x68) {
        v[0] = 104.0f - p->lx;
    } else if (p->lx >= 0x99) {
        v[0] = 152.0f - p->lx;
    } else {
        v[0] = 0.0f;
    }
    if (p->ly < 0x68) {
        v[2] = p->ly - 104.0f;
    } else if (p->ly >= 0x99) {
        v[2] = p->ly - 152.0f;
    } else {
        v[2] = 0.0f;
    }
    v[1] = v[3] = 0.0f;
    VuMtxSetRotY(m, (s32)(c->ry * 10430.378f));
    APPLY_MATRIX(v, m, v);
    c->x.f += v[0] * 0.25f;
    c->z += v[2] * 0.25f;
    if (p->ry < 0x50) {
        c->rx -= (f32)((0x50 - p->ry) * 2) * 9.58738e-05f;
    } else if (p->ry > 0xB0) {
        c->rx += (f32)((p->ry - 0xB0) * 2) * 9.58738e-05f;
    }
    if (p->rx < 0x50) {
        c->rz -= (f32)((0x50 - p->rx) * 2) * 9.58738e-05f;
    } else if (p->rx > 0xB0) {
        c->rz += (f32)((p->rx - 0xB0) * 2) * 9.58738e-05f;
    }
    c->y += p->unk10 >> 2;
    c->y -= p->unk11 >> 2;
    c->ry += (f32)(p->unk16 * 4) * 9.58738e-05f;
    c->ry -= (f32)(p->unk17 * 4) * 9.58738e-05f;
    c->dist += p->unkF >> 1;
    c->dist -= p->unkE >> 1;
    if (p->buttons & 1) {
        c->x.i = 0;
        c->y = 0.0f;
        c->z = 0.0f;
        c->rx = c->ry = c->rz = c->x.f;
        c->dist = 3000.0f;
    }
}
