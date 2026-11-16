/* Matches libgcc's divide / _fpdiv_parts (config/fp-bit.c, double). Third-party library code: see README.md, "Third-party library code". */
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

extern fp_number_type __thenan_df;

void __unpack_d(FLO_union_type *, fp_number_type *);
FLO_type __pack_d(fp_number_type *);

static __inline__ fp_number_type *_fpdiv_parts(fp_number_type *a, fp_number_type *b) {
    fractype bit;
    fractype numerator;
    fractype denominator;
    fractype quotient;

    if (fp_isnan(a)) {
        return a;
    }
    if (fp_isnan(b)) {
        return b;
    }

    a->sign = a->sign ^ b->sign;

    if (fp_isinf(a) || fp_iszero(a)) {
        if (a->class == b->class)
            return &__thenan_df;
        return a;
    }

    if (fp_isinf(b)) {
        a->fraction.ll = 0;
        a->normal_exp = 0;
        return a;
    }
    if (fp_iszero(b)) {
        a->class = CLASS_INFINITY;
        return a;
    }

    {
        a->normal_exp = a->normal_exp - b->normal_exp;
        numerator = a->fraction.ll;
        denominator = b->fraction.ll;

        if (numerator < denominator) {
            numerator *= 2;
            a->normal_exp--;
        }
        bit = IMPLICIT_1;
        quotient = 0;
        while (bit) {
            if (numerator >= denominator) {
                quotient |= bit;
                numerator -= denominator;
            }
            bit >>= 1;
            numerator *= 2;
        }

        if ((quotient & GARDMASK) == GARDMSB) {
            if (quotient & (1 << NGARDS)) {
                quotient += GARDROUND + 1;
            } else if (numerator) {
                quotient += GARDROUND + 1;
            }
        }

        a->fraction.ll = quotient;
        return (a);
    }
}

FLO_type dpdiv(FLO_type arg_a, FLO_type arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type *res;

    __unpack_d((FLO_union_type *)&arg_a, &a);
    __unpack_d((FLO_union_type *)&arg_b, &b);

    res = _fpdiv_parts(&a, &b);

    return __pack_d(res);
}
