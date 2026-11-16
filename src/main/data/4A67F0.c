/* main executable .rodata 0x004A67F0-0x004A6800: VuDirFromAngles' forward
 * vector. Data-only C (plan step 6b, docs/shifting.md); VuDirFromAngles
 * (0x00430738) is still asm. The word before it (D_004A67EC) is zero padding
 * up to this 16-aligned vector and stays a small asm piece.
 */
/* rodata: 0x004A67F0-0x004A6800 */

typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16))); /* vu.h */

/* (0, 0, 1, 1): rotated by the x/y angles into a direction */
const FVECTOR vuForwardVec = { 0.0f, 0.0f, 1.0f, 1.0f };
