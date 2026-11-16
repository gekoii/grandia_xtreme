/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct { s32 w[4]; } Unp;
void __unpack_f(f32 *, Unp *);
s32 _fpadd_parts_f(Unp *, Unp *, Unp *);
s32 __pack_f(s32);

s32 fpadd(f32 a, f32 b) {
    Unp ua;
    Unp ub;
    Unp ur;
    f32 fa;
    f32 fb;

    fa = a;
    fb = b;
    __unpack_f(&fa, &ua);
    __unpack_f(&fb, &ub);
    return __pack_f(_fpadd_parts_f(&ua, &ub, &ur));
}
