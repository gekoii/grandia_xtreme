/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    s32 pad;
    s64 d;
} S;

void __pack_d(S *);

void __make_dp(s32 arg0, s32 arg1, s32 arg2, s64 arg3) {
    S s;
    s.a = arg0;
    s.b = arg1;
    s.c = arg2;
    s.d = arg3;
    __pack_d(&s);
}
