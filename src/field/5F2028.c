#include "common.h"
#include "sys.h"



void PtyPartyGet(s16 *out) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (btlHandoff.unitIds[i] == 0) {
            out[i] = 0;
        } else {
            out[i] = btlHandoff.unitIds[i] - 0x1000;
        }
    }
}
