/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

void __pack_f(s32 *);

void __make_fp(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 buf[4];
    buf[0] = arg0;
    buf[1] = arg1;
    buf[2] = arg2;
    buf[3] = arg3;
    __pack_f(buf);
}
