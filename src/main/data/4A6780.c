/* main executable .rodata 0x004A6780-0x004A6790: VuBezier3's coefficients.
 * Data-only C (plan step 6b, docs/shifting.md); VuBezier3 (0x0042F278) is
 * still asm. The word before it (D_004A677C) is zero padding up to this
 * 16-aligned vector and stays a small asm piece.
 */
/* rodata: 0x004A6780-0x004A6790 */

typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16))); /* vu.h */

/* cubic Bezier weights (3, 3, 3, 1) for VuBezier3(out, p[4], t) */
const FVECTOR vuBezierCoef = { 3.0f, 3.0f, 3.0f, 1.0f };
