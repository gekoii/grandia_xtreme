/* Port-only C for VuMtxScale (0x00432CE0), which the decompilation still has in assembly. */
/*
 * VU0 registers: the PS2 leaves the scaled rows in $vf4-$vf7; the only C
 * caller with inline asm (src/main/433DE0.c) doesn't read them, so the
 * port doesn't write grx_vf.
 */
#include "common.h"
#include "vu0_port.h"

void VuMtxScale(f32 *m, f32 *s);

/* Rows 0, 1, 2 (all four lanes) times s.x, s.y, s.z; row 3 kept */
void VuMtxScale(f32 *m, f32 *s) {
    f32 v[4];
    s32 i;

    VuCopy4(v, s);
    for (i = 0; i < 4; i++) {
        m[0 + i] *= v[0];
        m[4 + i] *= v[1];
        m[8 + i] *= v[2];
    }
}
