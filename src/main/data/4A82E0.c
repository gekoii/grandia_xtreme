/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: newlib's s_infconst.c, +infinity as a constant (HUGE_VAL;
 * logf's wrapper 47B818.c uses it). Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here.
 */
/* rodata: 0x004A82E0-0x004A82E8 */

union __dmath {
    unsigned int i[2];
    double d;
};

const union __dmath __infinity[1] = { { { 0, 0x7FF00000 } } };
