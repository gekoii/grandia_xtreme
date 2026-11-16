#include "common.h"
#include "battle/unit.h"

void IpIconGetComboSpread(BtlUnit *arg0, s32 *arg1, f32 *arg2) {
    BtlUnit *u;
    s32 i;
    s8 cnt;
    s32 mask;
    s32 q;

    if (arg0->comboLeader >= 0) {
        u = &btlUnits[arg0->comboLeader];
    } else {
        u = arg0;
    }
    mask = 1 << u->id;
    cnt = 1;
    for (i = 0; i < 3 && u->comboPartners[i] >= 0; i++) {
        if (btlUnits[u->comboPartners[i]].action == 6) {
            cnt++;
            mask |= 1 << u->comboPartners[i];
        }
    }
    *arg1 = 0;
    for (i = 0; i < arg0->id; i++) {
        if ((mask >> i) & 1) {
            (*arg1)++;
        }
    }
    q = 0x10000 / cnt;
    *arg2 = cnt * 4.0f;
    *arg1 = *arg1 * q;
}
