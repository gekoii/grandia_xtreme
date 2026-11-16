/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's k_rem_pio2.c (__kernel_rem_pio2,
 * 47E748.c). Data-only C (plan step 6b, docs/shifting.md). The function
 * indexes init_jk and PIo2; zero, one, two24 and twon24 are folded into
 * literals but gcc 2.9 still emits the unused statics. Don't include the
 * project headers here.
 */
/* rodata: 0x004A8120-0x004A8190 */

/* initial value for jk */
const int init_jk[4] = { 2, 3, 4, 6 };

/* the first 24 bits of pi/2 in each double, chopped */
const double PIo2[8] = {
    1.57079625129699707031e+00, /* 0x3FF921FB, 0x40000000 */
    7.54978941586159635335e-08, /* 0x3E74442D, 0x00000000 */
    5.39030252995776476554e-15, /* 0x3CF84698, 0x80000000 */
    3.28200341580791294123e-22, /* 0x3B78CC51, 0x60000000 */
    1.27065575308067607349e-29, /* 0x39F01B83, 0x80000000 */
    1.22933308981111328932e-36, /* 0x387A2520, 0x40000000 */
    2.73370053816464559624e-44, /* 0x36E38222, 0x80000000 */
    2.16741683877804819444e-51, /* 0x3569F31D, 0x00000000 */
};

static const double zero = 0.0;
static const double one = 1.0;
static const double two24 = 1.67772160000000000000e+07;  /* 0x41700000, 0x00000000 */
static const double twon24 = 5.96046447753906250000e-08; /* 0x3E700000, 0x00000000 */
