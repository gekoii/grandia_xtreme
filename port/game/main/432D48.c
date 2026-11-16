/* Port-only C for VuMtxRotateQuat (0x00432D48), which the decompilation still has in assembly. */
/*
 * Also VuMtxApplyTRS (0x00432E58), VuMtxApplyTRSQuat (0x00432F28),
 * VuMtxApplyTRSEulerYXY (0x00433070) and VuMtxApplyTRSEulerZXY
 * (0x00433180), the T/R/S block routines of the same asm file. A T/R/S
 * block is three quadwords: translation, rotation (16-bit euler angles as
 * s32, or a quaternion), scale. flags bit 0 translates, bit 1 rotates,
 * bit 2 scales.
 *
 * VU0 register contract (VuMtxApplyTRSQuat): the asm keeps the matrix in
 * $vf16-$vf19 and only loads it from m when flags bit 0 is set, and only
 * stores it back to m when bit 2 is set. So without bit 0 it works on
 * whatever $vf16-$vf19 hold, and without bit 2 its result is left in
 * $vf16-$vf19 only. The port reads and writes grx_vf[16..19] the same way.
 * (Its only callers, GphMotPartCalcMatrix 435130 and GphMotPartCalcMatrixEx
 * 435E90, pass 7.)
 *
 * VuMtxRotateQuat reads $vf19 without loading it, but only multiplied by
 * the w of its rotation rows, which is 0: no contract.
 */
#include "common.h"
#include "vu.h"
#include "protos/main.h"
#include "vu0_port.h"

/* Scratchpad staging copy of the quaternion (it is copied word by word so
 * the source needn't be 16-byte aligned) */
#define SPR_QUAT ((f32 *)0x70003000)

void VuMtxRotate(f32 *mtx, void *angles);
void VuMtxSetRotY(f32 *m, s32 angle);
void VuMtxSetRotZ(f32 *m, s32 angle);
void VuMtxMul(f32 *dst, f32 *a, f32 *b);

void VuMtxRotateQuat(f32 *m, f32 *q);
void VuMtxApplyTRS(s32 flags, f32 *m, f32 *trs);
void VuMtxApplyTRSQuat(s32 flags, f32 *m, f32 *trs);
void VuMtxApplyTRSEulerYXY(s32 flags, f32 *m, f32 *trs);
void VuMtxApplyTRSEulerZXY(s32 flags, f32 *m, f32 *trs);

/* The rotation rows of quaternion q, stored (w, x, y, z): the asm rotates
 * it to (x, y, z, w) with vmr32. Row w components are 0. */
static void QuatRows(f32 r[3][4], const f32 *q) {
    f32 x = q[1];
    f32 y = q[2];
    f32 z = q[3];
    f32 w = q[0];

    r[0][0] = 1.0f - (y * y + z * z) * 2.0f;
    r[0][1] = (x * y + z * w) * 2.0f;
    r[0][2] = (x * z - w * y) * 2.0f;
    r[0][3] = 0.0f;
    r[1][0] = (x * y - z * w) * 2.0f;
    r[1][1] = 1.0f - (z * z + x * x) * 2.0f;
    r[1][2] = (y * z + x * w) * 2.0f;
    r[1][3] = 0.0f;
    r[2][0] = (x * z + w * y) * 2.0f;
    r[2][1] = (y * z - x * w) * 2.0f;
    r[2][2] = 1.0f - (y * y + x * x) * 2.0f;
    r[2][3] = 0.0f;
}

/* Rows 0-2 of m = rotation rows times rows 0-2 of m (vmaddz: row 3 not
 * involved, w of the rotation rows is 0) */
static void RotateRows(f32 *m, const f32 *q) {
    f32 r[3][4];
    f32 out[12];
    s32 i;
    s32 j;

    QuatRows(r, q);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            out[i * 4 + j] = m[j] * r[i][0] + m[4 + j] * r[i][1] + m[8 + j] * r[i][2];
        }
    }
    for (i = 0; i < 12; i++) {
        m[i] = out[i];
    }
}

/* Row 3 xyz = (t.xyz, 1) * m; row 3 w kept */
static void Translate(f32 *m, const f32 *t) {
    f32 v[4];

    VuCopy4(v, t);
    v[3] = 1.0f;
    VuApplyMtx(v, v, m);
    m[12] = v[0];
    m[13] = v[1];
    m[14] = v[2];
}

/* Rows 0, 1, 2 (all lanes) times s.x, s.y, s.z */
static void Scale(f32 *m, const f32 *s) {
    f32 v[4];
    s32 i;

    VuCopy4(v, s);
    for (i = 0; i < 4; i++) {
        m[0 + i] *= v[0];
        m[4 + i] *= v[1];
        m[8 + i] *= v[2];
    }
}

/* Rotates m's 3x3 by quaternion q (rows 0-2 written, row 3 untouched) */
void VuMtxRotateQuat(f32 *m, f32 *q) {
    f32 v[4];

    SPR_QUAT[0] = q[0];
    SPR_QUAT[1] = q[1];
    SPR_QUAT[2] = q[2];
    SPR_QUAT[3] = q[3];
    VuCopy4(v, SPR_QUAT);
    RotateRows(m, v);
}

void VuMtxApplyTRS(s32 flags, f32 *m, f32 *trs) {
    if (flags & 1) {
        Translate(m, &trs[0]);
    }
    if ((flags >> 1) & 1) {
        VuMtxRotate(m, &trs[4]);
    }
    if ((flags >> 2) & 1) {
        Scale(m, &trs[8]);
    }
}

/* See the register contract above */
void VuMtxApplyTRSQuat(s32 flags, f32 *m, f32 *trs) {
    f32 r[16];
    f32 q[4];

    if (flags & 1) {
        VuCopyMtx(r, m);
        Translate(r, &trs[0]);
    } else {
        VuCopyMtx(r, &grx_vf[16]);
    }
    if ((flags >> 1) & 1) {
        VuCopy4(q, &trs[4]);
        RotateRows(r, q);
    }
    if ((flags >> 2) & 1) {
        Scale(r, &trs[8]);
        VuCopyMtx(m, r);
    }
    VuCopyMtx(&grx_vf[16], r);
}

/* Rotation rotY(rz) * rotX(rx) * rotY(ry) of the s32 angles at trs + 0x10 */
void VuMtxApplyTRSEulerYXY(s32 flags, f32 *m, f32 *trs) {
    FMATRIX rot;
    s32 *ang = (s32 *)&trs[4];

    if (flags & 1) {
        Translate(m, &trs[0]);
    }
    if ((flags >> 1) & 1) {
        VuMtxSetRotY(&rot[0][0], ang[2]);
        VuMtxRotX((s32)rot, ang[0], (s32)rot);
        VuMtxRotY((s32)rot, ang[1], (s32)rot);
        VuMtxMul(m, m, &rot[0][0]);
    }
    if ((flags >> 2) & 1) {
        Scale(m, &trs[8]);
    }
}

/* Rotation rotZ(rz) * rotX(rx) * rotY(ry) */
void VuMtxApplyTRSEulerZXY(s32 flags, f32 *m, f32 *trs) {
    FMATRIX rot;
    s32 *ang = (s32 *)&trs[4];

    if (flags & 1) {
        Translate(m, &trs[0]);
    }
    if ((flags >> 1) & 1) {
        VuMtxSetRotZ(&rot[0][0], ang[2]);
        VuMtxRotX((s32)rot, ang[0], (s32)rot);
        VuMtxRotY((s32)rot, ang[1], (s32)rot);
        VuMtxMul(m, m, &rot[0][0]);
    }
    if ((flags >> 2) & 1) {
        Scale(m, &trs[8]);
    }
}
