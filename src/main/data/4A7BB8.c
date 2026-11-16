/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's ef_asin.c (__ieee754_asinf,
 * 47CA20.c) and ef_atan2.c (__ieee754_atan2f, 47CDC0.c). Data-only C (plan
 * step 6b, docs/shifting.md). Split from 4A7B80.c (ef_acos.c) at the
 * original file boundary; ef_atan2.c's part starts at 0x004A7BF4 (4 mod 8),
 * so it can't be a file of its own. The functions fold the static
 * constants into li.s literals; gcc 2.9 still emits the unused statics, in
 * definition order. Don't include the project headers here.
 */
/* rodata: 0x004A7BB8-0x004A7C10 */

/* ef_asin.c */
static const float one = 1.0000000000e+00f;     /* 0x3F800000 */
static const float huge = 1.000e+30f;           /* 0x7149F2CA */
static const float pio2_hi = 1.5707962513e+00f; /* 0x3FC90FDA */
static const float pio2_lo = 7.5497894159e-08f; /* 0x33A22168 */
static const float pio4_hi = 7.8539818525e-01f; /* 0x3F490FDB */
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

/* ef_atan2.c */
static const float tiny = 1.0e-30f;             /* 0x0DA24260 */
static const float zero = 0.0f;
static const float pi_o_4 = 7.8539818525e-01f;  /* 0x3F490FDB */
static const float pi_o_2 = 1.5707963705e+00f;  /* 0x3FC90FDB */
static const float pi = 3.1415925026e+00f;      /* 0x40490FDA */
static const float pi_lo = 1.5099578832e-07f;   /* 0x34222168 */
/* compiler literal: -zero, returned by __ieee754_atan2f (47CDC0.c) */
const float __atan2f_mzero = -0.0f;
