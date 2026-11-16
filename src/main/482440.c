/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"
typedef struct { s32 w[4]; } Unp;
void __unpack_f(f32 *, Unp *);
s32 __fpcmp_parts_f(Unp *, Unp *);
s32 fpcmp(f32 a, f32 b) {
    Unp ua;
    Unp ub;
    f32 fa;
    f32 fb;
    fa = a;
    fb = b;
    __unpack_f(&fa, &ua);
    __unpack_f(&fb, &ub);
    return __fpcmp_parts_f(&ua, &ub);
}
