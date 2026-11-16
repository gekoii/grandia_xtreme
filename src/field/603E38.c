#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapSwitchFlagsUpdate(AmapWork *w) {
    s32 g;
    s32 j;
    s32 k;
    s32 ok;
    u16 key;
    u8 n;
    s32 m;

    if (w->result != 0) {
        return;
    }
    EvtFlagSetByType(5, 4, 0);
    if (EvtFlagGetByType(5, 0) && EvtFlagGetByType(5, 1) && EvtFlagGetByType(5, 2) && EvtFlagGetByType(5, 3)) {
        EvtFlagSetByType(5, 4, 1);
    }
    for (g = 0; g < w->groupCount; g++) {
        m = w->groups[g].gateCount;
        n = w->groups[g].switchCount;
        for (j = 0; j < m; j++) {
            ok = 1;
            key = w->groups[g].gates[j].key;
            for (k = 0; k < n; k++) {
                if (w->groups[g].switches[k].key == key && EvtFlagGetByType(5, w->groups[g].switches[k].id + 16) == 0) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                EvtFlagSetByType(5, g * 2 + j + 6, 1);
            }
        }
    }
}
