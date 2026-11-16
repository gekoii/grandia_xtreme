/* Matches libgcc's __floatsidf (config/fp-bit.c), which Sony's ee-gcc calls litodp. Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 class;
    u32 sign;
    s32 normal_exp;
    u64 fraction;
} fp_number_type;

s64 __pack_d(fp_number_type *);

s64 litodp(s32 arg_a) {
    fp_number_type in;

    in.class = 3;
    in.sign = arg_a < 0;
    if (!arg_a) {
        in.class = 2;
    } else {
        in.normal_exp = 60;
        if (in.sign) {
            if (arg_a == (-0x7FFFFFFF - 1)) {
                return 0xC1E0000000000000LL;
            }
            in.fraction = -arg_a;
        } else {
            in.fraction = arg_a;
        }
        while (in.fraction < (1ULL << 60)) {
            in.fraction <<= 1;
            in.normal_exp -= 1;
        }
    }
    return __pack_d(&in);
}
