/* Matches libgcc's unpack_d (config/fp-bit.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef s32 SItype;
typedef u32 USItype;
typedef f32 SFtype;
typedef SFtype FLO_type;
typedef USItype fractype;
typedef u16 halffractype;
typedef SItype intfrac;

#define NGARDS 7L
#define GARDROUND 0x3f
#define GARDMASK 0x7f
#define GARDMSB 0x40
#define EXPBITS 8
#define EXPBIAS 127
#define FRACBITS 23
#define EXPMAX (0xff)
#define QUIET_NAN 0x100000L
#define FRAC_NBITS 32
#define IMPLICIT_1 (1L << (FRACBITS + NGARDS))
#define IMPLICIT_2 (1L << (FRACBITS + 1 + NGARDS))
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
    struct {
        fractype fraction : FRACBITS __attribute__((packed));
        unsigned int exp : EXPBITS __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} FLO_union_type;

#define isnan(x) ((x)->class == CLASS_QNAN || (x)->class == CLASS_SNAN)
#define isinf(x) ((x)->class == CLASS_INFINITY)
#define iszero(x) ((x)->class == CLASS_ZERO)

void __unpack_f(FLO_union_type *src, fp_number_type *dst) {
    fractype fraction;
    int exp;
    int sign;

    fraction = src->bits.fraction;
    exp = src->bits.exp;
    sign = src->bits.sign;

    dst->sign = sign;
    if (exp == 0) {
        dst->class = CLASS_ZERO;
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
