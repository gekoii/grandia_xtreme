/* cflags: -fno-schedule-insns */
/* Matches libgcc's __extendsfdf2 (config/fp-bit.c), which Sony's ee-gcc calls fptodp. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
void __unpack_f(f32 *, u32 *);

void fptodp(f32 arg0) {
    u32 s[4];
    f32 x;
    u32 *p = s;
    x = arg0;
    __unpack_f(&x, p);
    __make_dp(p[0], p[1], p[2], (u64)p[3] << 30);
}
