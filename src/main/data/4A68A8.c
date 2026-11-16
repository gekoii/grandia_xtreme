/* main executable .rodata 0x004A68A8-0x004A68F0: the unit corners of a 3D
 * quad (GphQuad3DSetCorners, 0x0044A0D0, still asm). Data-only C (plan step
 * 6b, docs/shifting.md). The table is a 16-aligned VU0 matrix; it can't be
 * declared aligned(16) here, because the range starts 8 bytes before it, so
 * those 8 zero bytes are an object of their own (D_004A68A8, nothing refers
 * to it). The word before the range (D_004A68A4, more zero padding) stays a
 * small asm piece.
 */
/* rodata: 0x004A68A8-0x004A68F0 */

typedef unsigned int u32;
typedef float f32;

const u32 D_004A68A8[2] = { 0, 0 };

/* x, y, z, w of the 4 corners, scaled by the quad's size */
const f32 gphQuadCorners[4][4] = {
    { -1.0f,  1.0f, 1.0f, 0.0f },
    { -1.0f, -1.0f, 1.0f, 0.0f },
    {  1.0f,  1.0f, 1.0f, 0.0f },
    {  1.0f, -1.0f, 1.0f, 0.0f },
};
