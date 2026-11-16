/* as: ee-gcc2.96-hazard */
/* rodata: 0x00674140-0x00674180 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void ChrExtraPartParticleSpawn(ChrObj *c, u8 kind, f32 r) {
    FVECTOR v;
    char *name = "save";
    u8 tbl[9] = { 0, 5, 3, 2, 7, 4, 0, 1, 6 };
    GphModel *m;
    GphPart *e;
    s32 n;

    if (!(c->flags & 1)) {
        return;
    }
    m = c->model;
    if (m == NULL) {
        return;
    }
    if (m->flags & 8) {
        return;
    }
    if (camCtl.aiType == 2) {
        return;
    }
    e = ChrModelFindPart(m, 0x366);
    if (e == NULL) {
        return;
    }
    if (e->flags & 8) {
        return;
    }
    n = 8;
    if (c->speed > 5.0f) {
        n = 0x30;
    } else if (c->speed > 1.0f) {
        n = 0x10;
    }
    if (n == 0) {
        return;
    }
    v[0] = 1.0f;
    v[1] = 0.0f;
    v[2] = -(f32)VuRandInt(0x50) + -16.0f;
    v[3] = 1.0f;
    v[0] += VuRandSigned(r);
    v[1] += VuRandSigned(r);
    v[2] += VuRandSigned(16.0f);
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf4, 0x0(%0)\n\t"
        "lqc2 $vf5, 0x10(%0)\n\t"
        "lqc2 $vf6, 0x20(%0)\n\t"
        "lqc2 $vf7, 0x30(%0)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0(%1)"
        : : "r"(e->mtx), "r"(v) : "memory");
    kind = tbl[kind];
    switch (kind) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            FldSpriteSpawn(kind, v, 0, 0x80, n);
            break;
        case 5:
            FldSpriteSpawn(kind, v, 0, 2, n);
            break;
        case 6:
            FldSpriteSpawn(7, v, 0, 0x80, n);
            break;
        case 7:
            FldSpriteSpawn(8, v, 0, 0x80, n);
            break;
    }
}
