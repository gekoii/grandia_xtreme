/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's sf_scalbn.c (scalbnf): its
 * static constants. Data-only C (plan step 6b, docs/shifting.md). The
 * function folds them into li.s literals; gcc 2.9 still emits the unused
 * statics. Don't include the project headers here.
 */
/* rodata: 0x004A8330-0x004A8340 */

static const float two25 = 3.355443200e+07f;   /* 0x4C000000 */
static const float twom25 = 2.9802322388e-08f; /* 0x33000000 */
static const float huge = 1.0e+30f;
static const float tiny = 1.0e-30f;
