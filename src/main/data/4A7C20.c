/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's ef_log.c (__ieee754_logf,
 * 47D2F8.c). Data-only C (plan step 6b, docs/shifting.md). Split from
 * 4A7B80.c at the original file boundary. The function folds the static
 * constants into li.s literals; gcc 2.9 still emits the unused statics, in
 * definition order. Don't include the project headers here.
 */
/* rodata: 0x004A7C20-0x004A7C50 */

static const float ln2_hi = 6.9313812256e-01f; /* 0x3F317180 */
static const float ln2_lo = 9.0580006145e-06f; /* 0x3717F7D1 */
static const float two25 = 3.355443200e+07f;   /* 0x4C000000 */
static const float Lg1 = 6.6666668653e-01f;    /* 0x3F2AAAAB */
static const float Lg2 = 4.0000000596e-01f;    /* 0x3ECCCCCD */
static const float Lg3 = 2.8571429849e-01f;    /* 0x3E924925 */
static const float Lg4 = 2.2222198546e-01f;    /* 0x3E638E29 */
static const float Lg5 = 1.8183572590e-01f;    /* 0x3E3A3325 */
static const float Lg6 = 1.5313838422e-01f;    /* 0x3E1CD04F */
static const float Lg7 = 1.4798198640e-01f;    /* 0x3E178897 */
static const float zero = 0.0f;
/* compiler literal: -two25/zero (-inf), returned by __ieee754_logf for
 * x == 0; u32 because C has no infinity literal */
const unsigned int __logf_minf = 0xFF800000;
