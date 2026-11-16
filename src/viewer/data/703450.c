/* Matches newlib's ef_acos.c (its static constants). Third-party library code: see README.md, "Third-party library code". */
/* VIEWER .rodata 0x00703450-0x00703488: data-only C (plan step 6b, docs/shifting.md).
 * The constants of newlib's ef_acos.c, whose __ieee754_acosf is
 * vwr___ieee754_acosf (src/viewer/7029D8.c). That file folds them into
 * literals; gcc 2.9 still emits the unused statics, in definition order.
 * The asm label D_00703450 (no users) is `one`, defined relative to the
 * file's __rodata_ marker. Drafted by tools/gen_data_c.py; check with
 * tools/check_data.py. Don't include the project headers here.
 */
/* rodata: 0x00703450-0x00703488 */

typedef float f32;

static const f32 one = 1.0000000000e+00f;      /* 0x3F800000; D_00703450 */
static const f32 pi = 3.1415925026e+00f;       /* 0x40490FDA */
static const f32 pio2_hi = 1.5707962513e+00f;  /* 0x3FC90FDA */
static const f32 pio2_lo = 7.5497894159e-08f;  /* 0x33A22168 */
static const f32 pS0 = 1.6666667163e-01f;      /* 0x3E2AAAAB */
static const f32 pS1 = -3.2556581497e-01f;     /* 0xBEA6B090 */
static const f32 pS2 = 2.0121252537e-01f;      /* 0x3E4E0AA8 */
static const f32 pS3 = -4.0055535734e-02f;     /* 0xBD241146 */
static const f32 pS4 = 7.9153501429e-04f;      /* 0x3A4F7F04 */
static const f32 pS5 = 3.4793309169e-05f;      /* 0x3811EF08 */
static const f32 qS1 = -2.4033949375e+00f;     /* 0xC019D139 */
static const f32 qS2 = 2.0209457874e+00f;      /* 0x4001572D */
static const f32 qS3 = -6.8828397989e-01f;     /* 0xBF303361 */
static const f32 qS4 = 7.7038154006e-02f;      /* 0x3D9DC62E */
