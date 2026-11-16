/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's s_rint.c (rint, 480508.c).
 * Data-only C (plan step 6b, docs/shifting.md). Don't include the project
 * headers here.
 */
/* rodata: 0x004A82F0-0x004A8300 */

const double TWO52[2] = {
    4.50359962737049600000e+15,  /* 0x43300000, 0x00000000 */
    -4.50359962737049600000e+15, /* 0xC3300000, 0x00000000 */
};
