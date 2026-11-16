#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlResultCountSe(s8 on) {
    if (on) {
        if (btlResultCountSe < 0) {
            btlResultCountSe = SndSePlay3D(0x7DA, 0, 0, NULL);
        }
    } else {
        if (btlResultCountSe >= 0) {
            SndSeStop(0, btlResultCountSe);
        }
        btlResultCountSe = -1;
    }
}
