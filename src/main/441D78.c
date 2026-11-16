#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


/* No parameters: 0x00400CB8 passes its u8 argument through a cast; the other callers pass none. */
void SndSeStopAll(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        SndSeStopGroup(var_s0 & 0xFF);
        var_s0 += 1;
    } while (var_s0 < 4);
}
