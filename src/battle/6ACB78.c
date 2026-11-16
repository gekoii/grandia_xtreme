#include "common.h"

void PtclBtlSprReset(s8 *arg0, s32 arg1) {
    s32 var_a1;
    s8 *var_a0;

    var_a0 = arg0;
    var_a1 = arg1;
    if (var_a1 > 0) {
        do {
            *var_a0 = 0;
            var_a1 -= 1;
            var_a0 += 0x40;
        } while (var_a1 > 0);
    }
}
