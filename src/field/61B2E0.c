#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


void EvtFaceWinCloseAll(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        EvtFaceWinClose(var_s0 & 0xFF);
        var_s0 += 1;
    } while (var_s0 < 3);
}
