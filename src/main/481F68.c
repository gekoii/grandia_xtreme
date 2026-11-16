/* Matches libgcc's __subsf3 (config/fp-bit.c). Third-party library code: see README.md, "Third-party library code". */
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

static __inline__ int fp_isnan(fp_number_type *x) {
    return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

static __inline__ int fp_isinf(fp_number_type *x) {
    return x->class == CLASS_INFINITY;
}

static __inline__ int fp_iszero(fp_number_type *x) {
    return x->class == CLASS_ZERO;
}

void __unpack_f(FLO_union_type *, fp_number_type *);
fp_number_type *_fpadd_parts_f(fp_number_type *, fp_number_type *, fp_number_type *);
FLO_type __pack_f(fp_number_type *);

FLO_type fpsub(FLO_type arg_a, FLO_type arg_b) {
    fp_number_type a;
    fp_number_type b;
    fp_number_type tmp;
    fp_number_type *res;

    __unpack_f((FLO_union_type *)&arg_a, &a);
    __unpack_f((FLO_union_type *)&arg_b, &b);

    b.sign ^= 1;

    res = _fpadd_parts_f(&a, &b, &tmp);

    return __pack_f(res);
}
