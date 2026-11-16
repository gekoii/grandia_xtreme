#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "party.h"

void PtyMoveLevelsUpdate(void);
void MemSet(void *, s32, s32);

void ItemPartyInit(s16 arg0) {
    s16 buf[4];
    s32 i;

    ItemUpdateLimits();
    if (arg0 == 1) {
        for (i = 0; i < 4; i++) {
            if (btlHandoff.unitIds[i] != 0) {
                PtyUnitInitFromChar(&ptyPartyUnits[i], btlHandoff.unitIds[i]);
                PtyUnitCalcParams(&ptyPartyUnits[i], 0, 0);
            }
        }
        return;
    }
    MemSet(ptyCharData, 0, 0x420);
    for (i = 3; i >= 0; i--) {
        buf[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        PtyCharInit(i + 0x1001, 0);
    }
    buf[0] = 1;
    PtyPartySet(buf);
    ItemInventoryClear();
    PtyMoveLevelsUpdate();
    ItemStartSetup();
}
