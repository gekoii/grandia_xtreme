/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's kf_rem_pio2.c
 * (__kernel_rem_pio2f, 47F5E0.c). Data-only C (plan step 6b,
 * docs/shifting.md). The function indexes init_jk and PIo2 (named
 * init_jkf/PIo2f here, apart from k_rem_pio2.c's); zero, one, two8 and
 * twon8 are folded into literals but gcc 2.9 still emits the unused
 * statics. Don't include the project headers here.
 */
/* rodata: 0x004A8220-0x004A826C */

/* initial value for jk */
const int init_jkf[3] = { 4, 7, 9 };

/* the first 8 bits of pi/2 in each float, chopped */
const float PIo2f[11] = {
    1.5703125000e+00f, /* 0x3FC90000 */
    4.5776367188e-04f, /* 0x39F00000 */
    2.5987625122e-05f, /* 0x37DA0000 */
    7.5437128544e-08f, /* 0x33A20000 */
    6.0026650317e-11f, /* 0x2E840000 */
    7.3896444519e-13f, /* 0x2B500000 */
    5.3845816694e-15f, /* 0x27C20000 */
    5.6378512969e-18f, /* 0x22D00000 */
    8.3009228831e-20f, /* 0x1FC40000 */
    3.2756352257e-22f, /* 0x1BC60000 */
    6.3331015649e-25f, /* 0x17440000 */
};

static const float zero = 0.0f;
static const float one = 1.0f;
static const float two8 = 2.5600000000e+02f;  /* 0x43800000 */
static const float twon8 = 3.9062500000e-03f; /* 0x3B800000 */
