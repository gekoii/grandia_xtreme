#include "common.h"

void BtlStrSpaceToZero(u8 *arg0) {
    u8 *var_a0;
    u8 var_v0;

    var_a0 = arg0;
    var_v0 = *var_a0;
    if (var_v0 != 0) {
        do {
            if (var_v0 == 0x20) {
                *var_a0 = 0x30;
            }
            var_a0 += 1;
            var_v0 = *var_a0;
        } while (var_v0 != 0);
    }
}
