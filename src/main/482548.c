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
        u32 ll;
    } fraction;
} fp_number_type;

#define isnan(x) ((x)->class == CLASS_SNAN || (x)->class == CLASS_QNAN)
#define isinf(x) (((x)->class ^ CLASS_INFINITY) == 0)
#define iszero(x) (((x)->class ^ CLASS_ZERO) == 0)

void __unpack_f(f32 *, fp_number_type *);

s32 fptosi(f32 arg_a) {
    fp_number_type a;
    s32 tmp;

    __unpack_f(&arg_a, &a);

    if (iszero(&a)) {
        return 0;
    }
    if (isnan(&a)) {
        return 0;
    }
    if (isinf(&a)) {
        return a.sign ? (-0x7FFFFFFF) - 1 : 0x7FFFFFFF;
    }
    if (a.normal_exp < 0) {
        return 0;
    }
    if (a.normal_exp > 30) {
        return a.sign ? (-0x7FFFFFFF) - 1 : 0x7FFFFFFF;
    }
    tmp = a.fraction.ll >> (30 - a.normal_exp);
    return a.sign ? -tmp : tmp;
}
