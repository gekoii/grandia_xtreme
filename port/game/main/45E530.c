/* Port-only C for VuLightTransform (0x0045E530), which the decompilation still has in assembly. */
#include "common.h"
#include "vu.h"
#include "vu0_port.h"

void VuVecCopy(f32 *dst, f32 *src);

void VuLightTransform(f32 *lights, f32 *mtx);

/*
 * The light block: four direction vectors at +0x110 transformed by mtx
 * without its translation, stored transposed at +0x10 (x), +0x20 (y),
 * +0x30 (z), with -w at +0x40; four positions at +0x150 transformed by the
 * whole matrix, transposed at +0x50.. with their own w at +0x80.
 *
 * Kept as the asm has it: a position's w for the transform is 1 for the
 * first, then the previous result's w (VuVecCopy only copies xyz into the
 * same stack vector).
 */
void VuLightTransform(f32 *lights, f32 *mtx) {
    FMATRIX rot;
    FVECTOR v;
    s32 k;

    VuCopyMtx(rot, mtx);
    rot[3][0] = 0.0f;
    rot[3][1] = 0.0f;
    rot[3][2] = 0.0f;
    for (k = 0; k < 4; k++) {
        VuApplyMtx(v, &lights[0x110 / 4 + k * 4], &rot[0][0]);
        lights[0x10 / 4 + k] = v[0];
        lights[0x20 / 4 + k] = v[1];
        lights[0x30 / 4 + k] = v[2];
        lights[0x40 / 4 + k] = -v[3];
    }
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;
    v[3] = 1.0f;
    for (k = 0; k < 4; k++) {
        VuVecCopy(v, &lights[0x150 / 4 + k * 4]);
        VuApplyMtx(v, v, mtx);
        lights[0x50 / 4 + k] = v[0];
        lights[0x60 / 4 + k] = v[1];
        lights[0x70 / 4 + k] = v[2];
        lights[0x80 / 4 + k] = lights[0x15C / 4 + k * 4];
    }
}
