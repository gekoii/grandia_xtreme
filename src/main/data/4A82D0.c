/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's s_floor.c (floor, 480308.c).
 * Data-only C (plan step 6b, docs/shifting.md). The static huge is unused
 * (gcc 2.9 still emits it); the function loads the literal it was folded
 * into, __floor_huge. Don't include the project headers here.
 */
/* rodata: 0x004A82D0-0x004A82E0 */

static const double huge = 1.0e300; /* 0x7E37E43C, 0x8800759C */
/* compiler literal: huge, for "huge + x > 0.0" */
const double __floor_huge = 1.0e300;
