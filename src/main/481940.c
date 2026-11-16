/* Matches libgcc's __fixdfsi (config/fp-bit.c), which Sony's ee-gcc calls dptoli. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u32 class;
    u32 sign;
    s32 normal_exp;
    union {
        u64 ll;
        u32 l[2];
    } fraction;
} fp_number_type;

typedef union {
    double value;
    u64 value_raw;
} FLO_union_type;

extern void __unpack_d(FLO_union_type *, fp_number_type *);

static __inline__ s32 fp_iszero(fp_number_type *x) {
    return x->class == 2;
}

static __inline__ s32 fp_isnan(fp_number_type *x) {
    return x->class == 0 || x->class == 1;
}

static __inline__ s32 fp_isinf(fp_number_type *x) {
    return x->class == 4;
}

s32 dptoli(double arg_a) {
    fp_number_type a;
    FLO_union_type au;
    s32 tmp;

    au.value = arg_a;
    __unpack_d(&au, &a);
    if (fp_iszero(&a)) {
        return 0;
    }
    if (fp_isnan(&a)) {
        return 0;
    }
    if (fp_isinf(&a)) {
        return a.sign ? (-0x7FFFFFFF) - 1 : 0x7FFFFFFF;
    }
    if (a.normal_exp < 0) {
        return 0;
    }
    if (a.normal_exp > 30) {
        return a.sign ? (-0x7FFFFFFF) - 1 : 0x7FFFFFFF;
    }
    tmp = a.fraction.ll >> (60 - a.normal_exp);
    return a.sign ? (-tmp) : (tmp);
}
