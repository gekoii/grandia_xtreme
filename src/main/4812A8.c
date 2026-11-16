/* Matches libgcc's __subdf3 (config/fp-bit.c), which Sony's ee-gcc calls dpsub. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 class;
    u32 sign;
    s32 normal_exp;
    u64 fraction;
} fp_number_type;

typedef union {
    double value;
    u64 value_raw;
} FLO_union_type;

void __unpack_d(FLO_union_type *, fp_number_type *);
fp_number_type *_fpadd_parts_d(fp_number_type *, fp_number_type *, fp_number_type *);
double __pack_d(fp_number_type *);

double dpsub(double arg_a, double arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type tmp;
    fp_number_type *res;
    FLO_union_type au, bu;

    au.value = arg_a;
    bu.value = arg_b;
    __unpack_d(&au, &a);
    __unpack_d(&bu, &b);
    b.sign ^= 1;
    res = _fpadd_parts_d(&a, &b, &tmp);
    return __pack_d(res);
}
