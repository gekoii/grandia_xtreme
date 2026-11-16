/* Matches libgcc's unpack_d (config/fp-bit.c, double). Third-party library code: see README.md, "Third-party library code". */
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

void __unpack_d(FLO_union_type *src, fp_number_type *dst) {
    fractype fraction;
    int exp;
    int sign;

    fraction = src->bits.fraction;
    exp = src->bits.exp;
    sign = src->bits.sign;

    dst->sign = sign;
    if (exp == 0) {
        if (fraction == 0 || 1) {
            dst->class = CLASS_ZERO;
        } else {
            dst->normal_exp = exp - EXPBIAS + 1;
            fraction <<= NGARDS;

            dst->class = CLASS_NUMBER;
            while (fraction < IMPLICIT_1) {
                fraction <<= 1;
                dst->normal_exp--;
            }
            dst->fraction.ll = fraction;
        }
    } else if (exp == EXPMAX) {
        if (fraction == 0) {
            dst->class = CLASS_INFINITY;
        } else {
            if (fraction & QUIET_NAN) {
                dst->class = CLASS_QNAN;
            } else {
                dst->class = CLASS_SNAN;
            }
            dst->fraction.ll = fraction;
        }
    } else {
        dst->normal_exp = exp - EXPBIAS;
        dst->class = CLASS_NUMBER;
        dst->fraction.ll = (fraction << NGARDS) | IMPLICIT_1;
    }
}
