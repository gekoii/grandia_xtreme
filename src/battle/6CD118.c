/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"


void BtlUnitStatePartHitWait(BtlUnit *arg0) {
    s32 st;
    f32 f;
    f32 g;

    st = arg0->subPhase;
    switch (st) {
    case 0:
        if ((btlUnits[arg0->body].params->status & 0x20E0) ||
            (arg0->params->status & 0x20E0)) {
            arg0->unk1D8 = 0.0f;
        } else {
            arg0->unk1D8 = -1.0f;
        }
        arg0->stateCount = 0;
        arg0->subPhase = st + 1;
    case 1:
        if (btlActingUnit == 0xFF) {
            f = arg0->unk1D8;
            if (f >= 0.0f) {
                g = f + 1.0f;
                arg0->unk1D8 = g;
                if (g >= 68.0f) {
                    BtlHudComboPopupAdd(arg0->id);
                    arg0->unk1D8 = -1.0f;
                }
            } else if (!(arg0->params->status & 0x20E0) && arg0->stateCount >= 0x44) {
                BtlUnitStateResume(arg0);
            }
        }
        break;
    }
}
