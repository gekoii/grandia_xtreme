/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's s_scalbn.c (scalbn, 480708.c).
 * Data-only C (plan step 6b, docs/shifting.md). The statics are unused
 * (gcc 2.9 still emits them); the function loads the literals tiny and
 * huge were folded into, __scalbn_tiny and __scalbn_huge. Don't include
 * the project headers here.
 */
/* rodata: 0x004A8300-0x004A8330 */

static const double two54 = 1.80143985094819840000e+16;  /* 0x43500000, 0x00000000 */
static const double twom54 = 5.55111512312578270212e-17; /* 0x3C900000, 0x00000000 */
static const double huge = 1.0e+300;
static const double tiny = 1.0e-300;
/* compiler literals */
const double __scalbn_tiny = 1.0e-300;
const double __scalbn_huge = 1.0e+300;
