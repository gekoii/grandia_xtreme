/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's k_sin.c (__kernel_sin,
 * 47F2B0.c): its static constants. Data-only C (plan step 6b,
 * docs/shifting.md). Split from 4A8120.c (k_rem_pio2.c) at the original
 * file boundary. The function folds them into literals; gcc 2.9 still
 * emits the unused statics, in definition order. Don't include the
 * project headers here.
 */
/* rodata: 0x004A8190-0x004A81C8 */

static const double half = 5.00000000000000000000e-01; /* 0x3FE00000, 0x00000000 */
static const double S1 = -1.66666666666666324348e-01;  /* 0xBFC55555, 0x55555549 */
static const double S2 = 8.33333333332248946124e-03;   /* 0x3F811111, 0x1110F8A6 */
static const double S3 = -1.98412698298579493134e-04;  /* 0xBF2A01A0, 0x19C161D5 */
static const double S4 = 2.75573137070700676789e-06;   /* 0x3EC71DE3, 0x57B1FE7D */
static const double S5 = -2.50507602534068634195e-08;  /* 0xBE5AE5E6, 0x8A2B9CEB */
static const double S6 = 1.58969099521155010221e-10;   /* 0x3DE5D93A, 0x5ACFD57C */
