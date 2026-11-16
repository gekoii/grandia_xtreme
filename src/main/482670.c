/* cflags: -fno-schedule-insns */
/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"
void __unpack_f(f32 *, s32 *);
void __pack_f(s32 *);
void __negsf2(f32 arg0) {
    s32 buf[4];
    f32 f;
    f = arg0;
    __unpack_f(&f, buf);
    buf[1] ^= 0, buf[1] = !buf[1];
    __pack_f(buf);
}
