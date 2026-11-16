/* Matches libgcc's __muldi3 (libgcc2.c), using longlong.h's umul_ppmm inline asm.
 * Third-party library code: it follows the upstream source and falls under
 * that library's license; see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 low;
    s32 high;
} DWstruct;

typedef union {
    DWstruct s;
    s64 ll;
} DWunion;

#define umul_ppmm(w1, w0, u, v) \
    __asm__("multu %2,%3" : "=l"((u32)(w0)), "=h"((u32)(w1)) : "d"((u32)(u)), "d"((u32)(v)))

#define __umulsidi3(u, v) \
    ({ DWunion __w; umul_ppmm(__w.s.high, __w.s.low, u, v); __w.ll; })

s64 __muldi3(s64 u, s64 v) {
    DWunion w;
    DWunion uu, vv;

    uu.ll = u;
    vv.ll = v;

    w.ll = __umulsidi3(uu.s.low, vv.s.low);
    w.s.high += ((u32)uu.s.low * (u32)vv.s.high + (u32)uu.s.high * (u32)vv.s.low);

    return w.ll;
}
