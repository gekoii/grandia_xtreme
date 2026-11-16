/* Port-only C for GphMeshClipTest (0x004577C8), which the decompilation still has in assembly. */
/*
 * The asm runs VU0 microprogram 0x340 (vuMicroCode0, assets/main/
 * 095A60.textbin.bin, at micro address 0x340). Disassembled, with the FMAC
 * latency of the FMAND reads accounted for, it computes, from the view
 * centre c ($vf8), the GA object's radius r ($vf9.x) and the clip block's
 * vectors a (+0x30, $vf10) and b (+0x40, $vf11):
 *     A = a * r, B = b * c.z,
 *     d1 = B - (c.x + A.x, c.x - A.y, c.y + A.z, c.y - A.w)
 *     d2 = B - (c.x - A.x, c.x + A.y, c.y - A.z, c.y + A.w)
 *     $vi2 = MAC sign flags of d1 & 0xF0, $vi3 = those of d2
 * (sign flags: 0x80 x, 0x40 y, 0x20 z, 0x10 w), which the EE compares with
 * 0xA0 (x and z negative, y and w not): the sphere is inside the side
 * planes (d1) or entirely inside them (d2). That is computed here in C.
 *
 * VU0 registers: the PS2 leaves $vf1, $vf4-$vf14 and $vi1-$vi3 changed; no
 * caller reads them (the callers' inline asm and the mesh draw routines
 * load their own), so the port doesn't write grx_vf/grx_vi. The transformed
 * centre is also stored to scratchpad 0x700002C0 as the asm does.
 */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

#define SPR_MESH_CENTER ((f32 *)0x700002C0)
#define SPR_CLIP_BLOCK (*(u8 **)0x700000EC)

s8 GphMeshClipTest(GphGaObject *obj);

/* MAC sign flags & 0xF0 */
static s32 SignFlags(const f32 *v) {
    return ((v[0] < 0.0f) << 7) | ((v[1] < 0.0f) << 6) | ((v[2] < 0.0f) << 5) | ((v[3] < 0.0f) << 4);
}

/*
 * -1: culled (beyond the near or far distance, or outside the side
 * planes). Else bit 0: crosses a side plane; bit 1: crosses the far
 * distance; 6 added when it crosses the near distance.
 */
s8 GphMeshClipTest(GphGaObject *obj) {
    u8 *clip = SPR_CLIP_BLOCK;
    f32 *mtx = (f32 *)obj->mtx;
    f32 c[4];
    f32 a[4];
    f32 b[4];
    f32 e1[4];
    f32 e2[4];
    f32 d1[4];
    f32 d2[4];
    f32 r;
    f32 zfar;
    f32 znear;
    f32 near;
    f32 far;
    s32 vi2;
    s32 vi3;
    s32 ret;
    s32 i;

    VuCopy4(c, obj->center);
    c[3] = 1.0f;
    VuApplyMtx(c, c, mtx);
    r = obj->radius;
    near = *(f32 *)(clip + 0x28);
    far = *(f32 *)(clip + 0x2C);
    VuCopy4(SPR_MESH_CENTER, c);
    VuCopy4(a, clip + 0x30);
    VuCopy4(b, clip + 0x40);

    /* Microprogram 0x340 */
    for (i = 0; i < 4; i++) {
        a[i] = a[i] * r;
        b[i] = b[i] * c[2];
    }
    e1[0] = c[0] + a[0];
    e1[1] = c[0] - a[1];
    e1[2] = c[1] + a[2];
    e1[3] = c[1] - a[3];
    e2[0] = c[0] - a[0];
    e2[1] = c[0] + a[1];
    e2[2] = c[1] - a[2];
    e2[3] = c[1] + a[3];
    for (i = 0; i < 4; i++) {
        d1[i] = b[i] - e1[i];
        d2[i] = b[i] - e2[i];
    }
    vi2 = SignFlags(d1);
    vi3 = SignFlags(d2);

    zfar = SPR_MESH_CENTER[2] + r;
    znear = SPR_MESH_CENTER[2] - r;
    if (zfar <= near) {
        return -1;
    }
    if (far <= znear) {
        return -1;
    }
    if (vi2 != 0xA0) {
        return -1;
    }
    if (far < zfar) {
        ret = (vi3 != 0xA0) ? 3 : 2;
    } else {
        ret = (vi3 != 0xA0) ? 1 : 0;
    }
    if (znear < near) {
        ret |= 6;
    }
    return ret;
}
