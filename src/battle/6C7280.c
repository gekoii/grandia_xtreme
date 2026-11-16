#include "common.h"
#include "protos/battle.h"


s32 BtlMenuPadDecide(s32 arg0) {
    if (arg0 & 0x40) {
        SndSePlay3D(0x7D3, 0, 0, 0);
        return -1;
    }
    if (arg0 & 0x20) {
        SndSePlay3D(0x7D2, 0, 0, 0);
        return 1;
    }
    return 0;
}
