/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef enum {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} fp_class_type;

typedef struct {
    fp_class_type class;
    u32 sign;
    s32 normal_exp;
    union {
        u64 ll;
    } fraction;
} fp_number_type;

#define isnan(x) ((x)->class == CLASS_SNAN || (x)->class == CLASS_QNAN)
#define isinf(x) (((x)->class ^ CLASS_INFINITY) == 0)
#define iszero(x) (((x)->class ^ CLASS_ZERO) == 0)

void __unpack_d(double *, fp_number_type *);

u32 dptoul(double arg_a) {
    fp_number_type a;

    __unpack_d(&arg_a, &a);

    if (iszero(&a)) {
        return 0;
    }
    if (isnan(&a)) {
        return 0;
    }
    if (a.sign) {
        return 0;
    }
    if (isinf(&a)) {
        return 0xFFFFFFFF;
    }
    if (a.normal_exp < 0) {
        return 0;
    }
    if (a.normal_exp > 31) {
        return 0xFFFFFFFF;
    } else if (a.normal_exp > 60) {
        return a.fraction.ll << (a.normal_exp - 60);
    } else {
        return a.fraction.ll >> (60 - a.normal_exp);
    }
}
