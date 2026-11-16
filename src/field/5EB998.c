#include "common.h"
#include "party.h"
#include "field/menu.h"

void ItemStatusParamsBuild(s16 idx) {
    PtyUnit *u;
    s32 i;
    s32 n;
    f32 f;

    u = &ptyPartyUnits[idx];
    n = 0;
    for (i = 0; i < 12; i++) {
        f = u->regSta[i];
        if (i != 9) {
            if (i != 10) {
                itemWork->statList[n].id = i + 1;
                itemWork->statList[n].val = f;
                n++;
            }
        }
    }
}
