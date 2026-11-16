/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
s32 _mbtowc_r(void *r, s32 *pwc, u8 *s, s32 n, int *arg4) {
    s32 dummy;
    if (pwc == NULL) pwc = &dummy;
    if (s == NULL) goto zero;
    if (n != 0) goto body;
    return -1;
zero:
    return 0;
body:
    *pwc = *s;
    return *s != 0;
}
