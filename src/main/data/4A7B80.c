/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's ef_acos.c (__ieee754_acosf,
 * 47C5F0.c): its static constants. Data-only C (plan step 6b,
 * docs/shifting.md). The function folds them into li.s literals; gcc 2.9
 * still emits the unused statics, in definition order. The asm label
 * D_004A7B80 (no users) is `one`, defined relative to the file's
 * __rodata_ marker. Don't include the project headers here.
 */
/* rodata: 0x004A7B80-0x004A7BB8 */

static const float one = 1.0000000000e+00f;     /* 0x3F800000; D_004A7B80 */
static const float pi = 3.1415925026e+00f;      /* 0x40490FDA */
static const float pio2_hi = 1.5707962513e+00f; /* 0x3FC90FDA */
static const float pio2_lo = 7.5497894159e-08f; /* 0x33A22168 */
static const float pS0 = 1.6666667163e-01f;     /* 0x3E2AAAAB */
static const float pS1 = -3.2556581497e-01f;    /* 0xBEA6B090 */
static const float pS2 = 2.0121252537e-01f;     /* 0x3E4E0AA8 */
static const float pS3 = -4.0055535734e-02f;    /* 0xBD241146 */
static const float pS4 = 7.9153501429e-04f;     /* 0x3A4F7F04 */
static const float pS5 = 3.4793309169e-05f;     /* 0x3811EF08 */
static const float qS1 = -2.4033949375e+00f;    /* 0xC019D139 */
static const float qS2 = 2.0209457874e+00f;     /* 0x4001572D */
static const float qS3 = -6.8828397989e-01f;    /* 0xBF303361 */
static const float qS4 = 7.7038154006e-02f;     /* 0x3D9DC62E */
