/* Matches libgcc's multiply / _fpmul_parts (config/fp-bit.c, double). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef s32 SItype;
typedef u32 USItype;
typedef s64 DItype;
typedef u64 UDItype;
typedef double DFtype;
typedef DFtype FLO_type;
typedef UDItype fractype;
typedef USItype halffractype;
typedef DItype intfrac;

#define NGARDS 8L
#define GARDROUND 0x7f
#define GARDMASK 0xff
#define GARDMSB 0x80
#define EXPBITS 11
#define EXPBIAS 1023
#define FRACBITS 52
#define EXPMAX (0x7ff)
#define QUIET_NAN 0x8000000000000LL
#define FRAC_NBITS 64
#define IMPLICIT_1 (1LL << (FRACBITS + NGARDS))
#define IMPLICIT_2 (1LL << (FRACBITS + 1 + NGARDS))
#define NORMAL_EXPMIN (-(EXPBIAS) + 1)
#define LSHIFT(a) { a = (a & 1) | (a >> 1); }

typedef enum {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} fp_class_type;

typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union {
        fractype ll;
        halffractype l[2];
    } fraction;
} fp_number_type;

typedef union {
    FLO_type value;
    fractype value_raw;
    halffractype words[2];
    struct {
        fractype fraction : FRACBITS __attribute__((packed));
        unsigned int exp : EXPBITS __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} FLO_union_type;

static __inline__ int fp_isnan(fp_number_type *x) {
    return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

static __inline__ int fp_isinf(fp_number_type *x) {
    return x->class == CLASS_INFINITY;
}

static __inline__ int fp_iszero(fp_number_type *x) {
    return x->class == CLASS_ZERO;
}

#define FRACHIGH 0x8000000000000000LL

extern fp_number_type __thenan_df;

void __unpack_d(FLO_union_type *, fp_number_type *);
FLO_type __pack_d(fp_number_type *);
UDItype func_00480DE0(UDItype, UDItype) __attribute__((const));

static __inline__ fp_number_type *_fpmul_parts(fp_number_type *a, fp_number_type *b, fp_number_type *tmp) {
    fractype low = 0;
    fractype high = 0;

    if (fp_isnan(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (fp_isnan(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }
    if (fp_isinf(a)) {
        if (fp_iszero(b))
            return &__thenan_df;
        a->sign = a->sign != b->sign;
        return a;
    }
    if (fp_isinf(b)) {
        if (fp_iszero(a)) {
            return &__thenan_df;
        }
        b->sign = a->sign != b->sign;
        return b;
    }
    if (fp_iszero(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (fp_iszero(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }

    {
        UDItype nl = a->fraction.ll & 0xffffffff;
        UDItype nh = a->fraction.ll >> 32;
        UDItype ml = b->fraction.ll & 0xffffffff;
        UDItype mh = b->fraction.ll >> 32;
        UDItype pp_ll = func_00480DE0(ml, nl);
        UDItype pp_hl = func_00480DE0(mh, nl);
        UDItype pp_lh = func_00480DE0(ml, nh);
        UDItype pp_hh = func_00480DE0(mh, nh);
        UDItype res2 = 0;
        UDItype res0 = 0;
        UDItype ps_hh__ = pp_hl + pp_lh;
        if (ps_hh__ < pp_hl)
            res2 += 0x100000000LL;
        pp_hl = (ps_hh__ << 32) & 0xffffffff00000000LL;
        res0 = pp_ll + pp_hl;
        if (res0 < pp_ll)
            res2++;
        res2 += ((ps_hh__ >> 32) & 0xffffffffL) + pp_hh;
        high = res2;
        low = res0;
    }

    tmp->normal_exp = a->normal_exp + b->normal_exp;
    tmp->sign = a->sign != b->sign;
    tmp->normal_exp += 4;
    while (high >= IMPLICIT_2) {
        tmp->normal_exp++;
        if (high & 1) {
            low >>= 1;
            low |= FRACHIGH;
        }
        high >>= 1;
    }
    while (high < IMPLICIT_1) {
        tmp->normal_exp--;

        high <<= 1;
        if (low & FRACHIGH)
            high |= 1;
        low <<= 1;
    }
    if ((high & GARDMASK) == GARDMSB) {
        if (high & (1 << NGARDS)) {
            high += GARDROUND + 1;
        } else if (low) {
            high += GARDROUND + 1;
        }
    }
    tmp->fraction.ll = high;
    tmp->class = CLASS_NUMBER;
    return tmp;
}

FLO_type dpmul(FLO_type arg_a, FLO_type arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type tmp;
    fp_number_type *res;

    __unpack_d((FLO_union_type *)&arg_a, &a);
    __unpack_d((FLO_union_type *)&arg_b, &b);

    res = _fpmul_parts(&a, &b, &tmp);

    return __pack_d(res);
}
