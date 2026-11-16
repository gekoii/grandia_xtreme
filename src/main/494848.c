/* Matches libgcc's __moddi3 (libgcc2.c), using longlong.h's umul_ppmm inline asm.
 * Third-party library code: it follows the upstream source and falls under
 * that library's license; see README.md, "Third-party library code". */
#include "common.h"

typedef unsigned int USItype;
typedef u64 UDItype;
struct DIstruct { s32 low, high; };
typedef union { struct DIstruct s; s64 ll; } DIunion;

extern const u8 __clz_tab_moddi3[];

#define __ll_B (1L << 16)
#define __ll_lowpart(t) ((USItype) (t) % __ll_B)
#define __ll_highpart(t) ((USItype) (t) / __ll_B)

#define count_leading_zeros(count, x) \
  do { \
    USItype __xr = (x); \
    USItype __a; \
    if (32 <= 32) \
      { \
    __a = __xr < ((USItype)1<<2*8) \
      ? (__xr < ((USItype)1<<8) ? 0 : 8) \
      : (__xr < ((USItype)1<<3*8) ?  2*8 : 3*8); \
      } \
    else \
      { \
        for (__a = 32 - 8; __a > 0; __a -= 8) \
          if (((__xr >> __a) & 0xff) != 0) \
            break; \
      } \
    (count) = 32 - (__clz_tab_moddi3[__xr >> __a] + __a); \
  } while (0)

#define udiv_qrnnd(q, r, n1, n0, d) \
  do { \
    USItype __d1, __d0, __q1, __q0; \
    USItype __r1, __r0, __m; \
    __d1 = __ll_highpart (d); \
    __d0 = __ll_lowpart (d); \
    __r1 = (n1) % __d1; \
    __q1 = (n1) / __d1; \
    __m = (USItype) __q1 * __d0; \
    __r1 = __r1 * __ll_B | __ll_highpart (n0); \
    if (__r1 < __m) \
      { \
        __q1--, __r1 += (d); \
        if (__r1 >= (d)) \
          if (__r1 < __m) \
            __q1--, __r1 += (d); \
      } \
    __r1 -= __m; \
    __r0 = __r1 % __d1; \
    __q0 = __r1 / __d1; \
    __m = (USItype) __q0 * __d0; \
    __r0 = __r0 * __ll_B | __ll_lowpart (n0); \
    if (__r0 < __m) \
      { \
        __q0--, __r0 += (d); \
        if (__r0 >= (d)) \
          if (__r0 < __m) \
            __q0--, __r0 += (d); \
      } \
    __r0 -= __m; \
    (q) = (USItype) (__q1 * __ll_B | __q0); \
    (r) = __r0; \
  } while (0)

#define umul_ppmm(w1, w0, u, v) \
__asm__("multu %2,%3" : "=l"((USItype)(w0)), "=h"((USItype)(w1)) : "d"((USItype)(u)), "d"((USItype)(v)))

#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do { \
    USItype __x; \
    __x = (al) - (bl); \
    (sh) = (ah) - (bh) - (__x > (al)); \
    (sl) = __x; \
  } while (0)

#define negdi2(res, x) \
  { \
    DIunion __w; \
    DIunion __uu; \
    __uu.ll = (x); \
    __w.s.low = -__uu.s.low; \
    __w.s.high = -__uu.s.high - ((USItype) __w.s.low > 0); \
    (res) = __w.ll; \
  }

s64 __moddi3(s64 a, s64 b_) {
    s64 c = 0;
    DIunion uu, vv;
    s64 w;

    uu.ll = a;
    vv.ll = b_;

    if (uu.s.high < 0) {
        c = ~c;
        negdi2(uu.ll, uu.ll);
    }
    if (vv.s.high < 0)
        negdi2(vv.ll, vv.ll);

    {
    UDItype u = uu.ll;
    UDItype v = vv.ll;
    UDItype *rp = (UDItype *) &w;
    DIunion ww;
    DIunion nn, dd;
    DIunion rr;
    USItype d0, d1, n0, n1, n2;
    USItype q0, q1;
    USItype b, bm;

    nn.ll = u;
    dd.ll = v;

    d0 = dd.s.low;
    d1 = dd.s.high;
    n0 = nn.s.low;
    n1 = nn.s.high;

    if (d1 == 0) {
        if (d0 > n1) {
            count_leading_zeros(bm, d0);
            if (bm != 0) {
                d0 = d0 << bm;
                n1 = (n1 << bm) | (n0 >> (32 - bm));
                n0 = n0 << bm;
            }
            udiv_qrnnd(q0, n0, n1, n0, d0);
            q1 = 0;
        } else {
            if (d0 == 0)
                d0 = 1 / d0;
            count_leading_zeros(bm, d0);
            if (bm == 0) {
                n1 -= d0;
                q1 = 1;
            } else {
                b = 32 - bm;
                d0 = d0 << bm;
                n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 = n0 << bm;
                udiv_qrnnd(q1, n1, n2, n1, d0);
            }
            udiv_qrnnd(q0, n0, n1, n0, d0);
        }
        if (rp != 0) {
            rr.s.low = n0 >> bm;
            rr.s.high = 0;
            *rp = rr.ll;
        }
    } else {
        if (d1 > n1) {
            q0 = 0;
            q1 = 0;
            if (rp != 0) {
                rr.s.low = n0;
                rr.s.high = n1;
                *rp = rr.ll;
            }
        } else {
            count_leading_zeros(bm, d1);
            if (bm == 0) {
                if (n1 > d1 || n0 >= d0) {
                    q0 = 1;
                    sub_ddmmss(n1, n0, n1, n0, d1, d0);
                } else
                    q0 = 0;
                q1 = 0;
                if (rp != 0) {
                    rr.s.low = n0;
                    rr.s.high = n1;
                    *rp = rr.ll;
                }
            } else {
                USItype m1, m0;
                b = 32 - bm;
                d1 = (d1 << bm) | (d0 >> b);
                d0 = d0 << bm;
                n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 = n0 << bm;
                udiv_qrnnd(q0, n1, n2, n1, d1);
                umul_ppmm(m1, m0, q0, d0);
                if (m1 > n1 || (m1 == n1 && m0 > n0)) {
                    q0--;
                    sub_ddmmss(m1, m0, m1, m0, d1, d0);
                }
                q1 = 0;
                if (rp != 0) {
                    sub_ddmmss(n1, n0, n1, n0, m1, m0);
                    rr.s.low = (n1 << b) | (n0 >> bm);
                    rr.s.high = n1 >> bm;
                    *rp = rr.ll;
                }
            }
        }
    }
    ww.s.low = q0;
    ww.s.high = q1;
    (void) ww.ll;
    }
    if (c)
        negdi2(w, w);

    return w;
}
