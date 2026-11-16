/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u32 class;
    u32 sign;
    s32 normal_exp;
    u32 fraction;
} fp_number_type;

f32 __pack_f(fp_number_type *);
void __unpack_f(f32 *, fp_number_type *);
extern fp_number_type __thenan_sf;

f32 fpdiv(f32 arg_a, f32 arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type *res;
    fp_number_type *pa;
    fp_number_type *pb;
    u32 bit;
    s32 t;
    u32 numerator;
    u32 denominator;
    u32 quotient;

    __unpack_f(&arg_a, &a);
    __unpack_f(&arg_b, &b);

    pa = &a;
    pb = &b;
    if (pa->class < 2) {
        res = pa;
        goto done;
    }
    if (b.class < 2) {
        res = pb;
        goto done;
    }
    pa->sign = pa->sign ^ b.sign;
    t = pa->class == 4;
    if (!t) {
        t = pa->class == 2;
    }
    if (t) {
        if (pa->class == b.class) {
            res = &__thenan_sf;
            goto done;
        }
        res = pa;
        goto done;
    }
    t = b.class == 4;
    if (t) {
        pa->fraction = 0;
        pa->normal_exp = 0;
        res = pa;
        goto done;
    }
    t = b.class == 2;
    if (t) {
        pa->class = 4;
        res = pa;
        goto done;
    }
    pa->normal_exp = pa->normal_exp - b.normal_exp;
    numerator = pa->fraction;
    denominator = b.fraction;
    if (numerator < denominator) {
        numerator *= 2;
        pa->normal_exp--;
    }
    bit = 0x40000000;
    quotient = 0;
    while (bit) {
        if (numerator >= denominator) {
            quotient |= bit;
            numerator -= denominator;
        }
        bit >>= 1;
        numerator *= 2;
    }
    if ((quotient & 0x7F) == 0x40) {
        if (quotient & 0x80) {
            quotient += 0x40;
        } else if (numerator) {
            quotient += 0x40;
        }
    }
    pa->fraction = quotient;
    res = pa;
done:
    return __pack_f(res);
}
