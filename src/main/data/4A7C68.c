/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's ef_pow.c (__ieee754_powf,
 * 47D750.c). Data-only C (plan step 6b, docs/shifting.md). The function
 * indexes bp, dp_h and dp_l (here under their symbol names) and loads the
 * other constants with li.s, so the static scalars are unused, but gcc 2.9
 * still emits them. Don't include the project headers here.
 */
/* rodata: 0x004A7C68-0x004A7CF0 */

const float __powf_bp[2] = { 1.0f, 1.5f };                   /* bp */
const float __powf_dp_h[2] = { 0.0f, 5.84960938e-01f };      /* dp_h, 0x3F15C000 */
const float __powf_dp_l[2] = { 0.0f, 1.56322085e-06f };      /* dp_l, 0x35D1CFDC */
static const float zero = 0.0f;
static const float one = 1.0f;
static const float two = 2.0f;
static const float two24 = 16777216.0f;                      /* 0x4B800000 */
static const float huge = 1.0e30f;
static const float tiny = 1.0e-30f;
/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
static const float L1 = 6.0000002384e-01f;                   /* 0x3F19999A */
static const float L2 = 4.2857143283e-01f;                   /* 0x3EDB6DB7 */
static const float L3 = 3.3333334327e-01f;                   /* 0x3EAAAAAB */
static const float L4 = 2.7272811532e-01f;                   /* 0x3E8BA305 */
static const float L5 = 2.3066075146e-01f;                   /* 0x3E6C3255 */
static const float L6 = 2.0697501302e-01f;                   /* 0x3E53F142 */
static const float P1 = 1.6666667163e-01f;                   /* 0x3E2AAAAB */
static const float P2 = -2.7777778450e-03f;                  /* 0xBB360B61 */
static const float P3 = 6.6137559770e-05f;                   /* 0x388AB355 */
static const float P4 = -1.6533901999e-06f;                  /* 0xB5DDEA0E */
static const float P5 = 4.1381369442e-08f;                   /* 0x3331BB4C */
static const float lg2 = 6.9314718246e-01f;                  /* 0x3F317218 */
static const float lg2_h = 6.93145752e-01f;                  /* 0x3F317200 */
static const float lg2_l = 1.42860654e-06f;                  /* 0x35BFBE8C */
static const float ovt = 4.2995665694e-08f;                  /* -(128-log2(ovfl+.5ulp)) */
static const float cp = 9.6179670095e-01f;                   /* 0x3F76384F = 2/(3ln2) */
static const float cp_h = 9.6179199219e-01f;                 /* 0x3F763800 = head of cp */
static const float cp_l = 4.7017383622e-06f;                 /* 0x369DC3A0 = tail of cp_h */
static const float ivln2 = 1.4426950216e+00f;                /* 0x3FB8AA3B = 1/ln2 */
static const float ivln2_h = 1.4426879883e+00f;              /* 0x3FB8AA00 = 16b 1/ln2 */
static const float ivln2_l = 7.0526075433e-06f;              /* 0x36ECA570 = 1/ln2 tail */
/* compiler literal: huge*huge folded to +inf; u32 because C has no
 * infinity literal */
const unsigned int __powf_inf = 0x7F800000;
