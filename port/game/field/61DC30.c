/* Port-only C for FldCollTriBuild (0x0061DC30), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"

/* Collision triangle (0x50 bytes; FldCollSphereTriTest reads it): the
   first corner, the two edges from it and the unit normal. */
typedef struct FldCollTri {
    /* 0x00 */ FVECTOR p0;
    /* 0x10 */ FVECTOR e1;      /* p1 - p0 */
    /* 0x20 */ FVECTOR e2;      /* p2 - p0 */
    /* 0x30 */ FVECTOR normal;  /* e2 x e1, normalised */
    /* 0x40 */ u8 pad40[0x10];
} FldCollTri;

void VuVecNormalize(f32 *, f32 *);
void FldCollTriBuild(FldCollTri *t, f32 *p0, f32 *p1, f32 *p2);

void FldCollTriBuild(FldCollTri *t, f32 *p0, f32 *p1, f32 *p2) {
    f32 *a;
    f32 *b;
    s32 i;

    __builtin_memcpy(t->p0, p0, 16);
    for (i = 0; i < 4; i++) {
        t->e1[i] = p1[i] - p0[i];
    }
    for (i = 0; i < 4; i++) {
        t->e2[i] = p2[i] - p0[i];
    }
    /* vopmula/vopmsub: e2 x e1; w keeps e2's */
    a = t->e2;
    b = t->e1;
    t->normal[0] = a[1] * b[2] - b[1] * a[2];
    t->normal[1] = a[2] * b[0] - b[2] * a[0];
    t->normal[2] = a[0] * b[1] - b[0] * a[1];
    t->normal[3] = a[3];
    VuVecNormalize(t->normal, t->normal);
}
