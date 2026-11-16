/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c[6];
} S;

void __unpack_d(s64 *, S *);
void __pack_d(S *);

void __negdf2(s64 arg0) {
    S s;
    __unpack_d(&arg0, &s);
    s.b = s.b == 0;
    __pack_d(&s);
}
