/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u32 class;
    u32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

extern f32 __pack_f(fp_number_type *);
extern void __unpack_f(f32 *, fp_number_type *);
extern fp_number_type __thenan_sf;

f32 fpmul(f32 arg_a, f32 arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type tmp;
    fp_number_type *res;
    f32 au;
    f32 bu;
    s32 t;
    fp_number_type *pt;
    u32 low;
    u32 high;
    s64 answer;

    au = arg_a;
    bu = arg_b;
    __unpack_f(&au, &a);
    __unpack_f(&bu, &b);
    pt = &tmp;

    t = a.class < 2;
    if (t) {
        a.sign = a.sign != b.sign;
        res = &a;
        goto done;
    }
    t = b.class < 2;
    if (t) {
        b.sign = a.sign != b.sign;
        res = &b;
        goto done;
    }
    t = a.class == 4;
    if (t) {
        t = b.class == 2;
    if (t) {
            res = &__thenan_sf;
            goto done;
        }
        a.sign = a.sign != b.sign;
        res = &a;
        goto done;
    }
    t = b.class == 4;
    if (t) {
        t = a.class == 2;
    if (t) {
            res = &__thenan_sf;
            goto done;
        }
        b.sign = a.sign != b.sign;
        res = &b;
        goto done;
    }
    t = a.class == 2;
    if (t) {
        a.sign = a.sign != b.sign;
        res = &a;
        goto done;
    }
    t = b.class == 2;
    if (t) {
        b.sign = a.sign != b.sign;
        res = &b;
        goto done;
    }
    answer = (s64)a.fraction * (s64)b.fraction;
    high = answer >> 32;
    low = answer;
    tmp.normal_exp = a.normal_exp + b.normal_exp;
    tmp.sign = a.sign != b.sign;
    tmp.normal_exp += 2;
    while (high >= 0x80000000) {
        tmp.normal_exp++;
        if (high & 1) {
            low >>= 1;
            low |= 0x80000000;
        }
        high >>= 1;
    }
    while (high < 0x40000000) {
        tmp.normal_exp--;
        high <<= 1;
        if (low & 0x80000000) {
            high |= 1;
        }
        low <<= 1;
    }
    if ((high & 0x7F) == 0x40) {
        if (high & 0x80) {
            high += 0x40;
        } else if (low) {
            high += 0x40;
        }
    }
    tmp.fraction = high;
    pt->class = 3;
    res = pt;
done:
    return __pack_f(res);
}
