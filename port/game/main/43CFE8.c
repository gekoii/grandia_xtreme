/* Port-only C for GphSphereDraw (0x0043CFE8), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"

extern u16 gphSphereSlices;
extern u16 gphSphereStacks;
extern f32 *gphSphereVerts;
s32 GphSphereClipTest(f32 (*m)[4], f32 *v);
void GphPoly3DDraw(u32 n, f32 *verts, u32 color, s32 mode);
void GphSphereDraw(u32 n, f32 *spheres, u32 color);

static f32 FromBits(u32 bits) {
    union {
        u32 u;
        f32 f;
    } v;

    v.u = bits;
    return v.f;
}

/* Draws n spheres (x, y, z, radius each) as gphSphereStacks triangle
   strips of gphSphereSlices + 1 vertex pairs. The sin/cos tables are
   built in the scratchpad from a quarter (sinf) by symmetry: S (sin) at
   0x70003B00 and A (cos) at 0x70003C00 around the slices, C (sin) at
   0x70003D00 and B (cos) at 0x70003D80 from pole to pole; the strips are
   built at 0x70003600 (gphSphereVerts). A sphere that fails the clip
   test is skipped without moving on to the next one, so the rest are
   tested at the same position: kept as the asm has it. */
void GphSphereDraw(u32 n, f32 *spheres, u32 color) {
    f32 *S = (f32 *)0x70003B00;
    f32 *A = (f32 *)0x70003C00;
    f32 *C = (f32 *)0x70003D00;
    f32 *B = (f32 *)0x70003D80;
    s32 slices = gphSphereSlices;
    s32 stacks = gphSphereStacks;
    s32 q = (u32)slices >> 2;
    s32 h = (u32)stacks >> 1;
    s32 i;
    u32 k, row, col, cols;
    f32 (*mtx)[4];
    f32 x, y, z, r, t;
    f32 *v;

    gphSphereVerts = (f32 *)0x70003600;
    for (i = 0; i <= q; i++) {
        S[i] = sinf((f32)i * (FromBits(0x40C90FD8) / (f32)slices));
    }
    for (i = 0; i <= q; i++) {
        A[i] = S[q - i];
    }
    for (i = 0; i <= q; i++) {
        S[q + i] = S[q - i];
    }
    for (i = 0; i <= q * 2; i++) {
        S[q * 4 - i] = -S[i];
    }
    for (i = 0; i <= q * 2; i++) {
        A[q + i] = S[q * 2 + i];
    }
    for (i = 0; i <= q; i++) {
        A[q * 4 - i] = A[i];
    }
    for (i = 0; i <= h; i++) {
        C[i] = sinf((f32)i * (FromBits(0x40490FD8) / (f32)stacks) + FromBits(0xBFC90FD8));
    }
    for (i = 0; i <= h; i++) {
        B[i] = -C[h - i];
    }
    for (i = 0; i <= h; i++) {
        C[h * 2 - i] = -C[i];
    }
    for (i = 0; i <= h; i++) {
        B[h * 2 - i] = B[i];
    }

    mtx = (f32 (*)[4])VuMtxStackGetTop();
    cols = slices + 1;
    for (k = 0; k < n; k++) {
        if (GphSphereClipTest(mtx, spheres) < 0) {
            continue;
        }
        x = spheres[0];
        y = spheres[1];
        z = spheres[2];
        r = spheres[3];
        for (row = 0; row < (u32)stacks; row++) {
            v = gphSphereVerts;
            for (col = 0; col < cols; col++) {
                t = r * B[row + 1];
                *v++ = x + t * A[col];
                *v++ = y + r * C[row + 1];
                *v++ = z + t * S[col];
                *v++ = 1.0f;
                t = r * B[row];
                *v++ = x + t * A[col];
                *v++ = y + r * C[row];
                *v++ = z + t * S[col];
                *v++ = 1.0f;
            }
            GphPoly3DDraw(cols * 2, gphSphereVerts, color, 1);
        }
        spheres += 4;
    }
}
