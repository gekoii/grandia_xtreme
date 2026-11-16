/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 class;
    u32 sign;
    s32 normal_exp;
    union {
        u32 ll;
    } fraction;
} fp_number_type;

f32 __pack_f(fp_number_type *);

f32 sitofp(s32 arg_a) {
    fp_number_type in;

    in.class = 3;
    in.sign = arg_a < 0;
    if (!arg_a) {
        in.class = 2;
    } else {
        in.normal_exp = 30;
        if (in.sign) {
            if (arg_a == (-0x7FFFFFFF - 1)) {
                return (f32)(-0x7FFFFFFF - 1);
            }
            in.fraction.ll = -arg_a;
        } else {
            in.fraction.ll = arg_a;
        }
        while (in.fraction.ll < (1 << 30)) {
            in.fraction.ll <<= 1;
            in.normal_exp -= 1;
        }
    }
    return __pack_f(&in);
}
