#include "common.h"
#include "battle/unit.h"

void BtlChrModelInvalidate(s32 arg0) {
    s32 var_v1;
    s8 *var_v0;

    if (arg0 >= 0x10) {
        var_v1 = 0xF;
        var_v0 = btlChrModelUpdated;
        do {
            *var_v0 = 0;
            var_v1 -= 1;
            var_v0 += 1;
        } while (var_v1 >= 0);
        return;
    }
    btlChrModelUpdated[arg0] = 0;
}
