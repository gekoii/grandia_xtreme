/* Third-party library code (newlib, fdlibm-derived): see README.md, "Third-party library code". */
/* main executable: newlib's s_lib_ver.c, the math wrappers' error mode
 * (acosf 47B3B0.c, logf 47B818.c, ...). Data-only C (plan step 6b,
 * docs/shifting.md). It is in .rodata, so here it is const (newlib's isn't;
 * see include/sdk/newlib.h). Don't include the project headers here.
 */
/* rodata: 0x004A82E8-0x004A82F0 */

/* _LIB_VERSION_TYPE: fdlibm_ieee = -1, _SVID_, _XOPEN_, _POSIX_ */
const int _LIB_VERSION = 1; /* _XOPEN_ */
