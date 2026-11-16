/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

s64 dpmul(s64, s64);
extern s64 __mprec_tens[];

s64 _mprec_log10(s32 arg0) {
    s64 r = 0x3FF0000000000000LL;

    if (arg0 < 0x18) {
        return __mprec_tens[arg0];
    }
    while (arg0 > 0) {
        arg0--;
        r = dpmul(r, 0x4024000000000000LL);
    }
    return r;
}
