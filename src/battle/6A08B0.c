/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n30.h"
#include "party.h"


void FxUnitDraw(void) {
    BtlUnit *u;
    s32 i;

    if (fxComboLink.parts != NULL) {
        FxComboLinkDraw();
    }
    if (fxCastSparkCount > 0) {
        FxCastSparkDraw();
    }
    FxRegenDraw();
    FxAntiMagicDraw();
    FxStunDraw();
    FxBindDraw();
    FxDefeatBurstDraw();
    if (!(btlHudHideFlags & 4)) {
        GphTexBind(0xEC);
        SprBegin(0.0f, 0.0f);
        for (i = 0, u = btlUnits; i < 16; i++, u++) {
            if (u->alpha > 0.0f && fxStatusIconAlpha[u->id] > 0.0f) {
                FxStatusIconsDraw(u);
            }
        }
        SprFlush();
    }
    GphTexBind(0xE9);
    SprBegin(0.0f, 0.0f);
    for (i = 0, u = btlUnits; i < 16; i++, u++) {
        if (u->alpha > 0.0f) {
            FxFailLabelDraw(u);
        }
    }
    SprFlush();
    for (i = 0, u = btlUnits; i < 16; i++, u++) {
        if (u->alpha > 0.0f && !(u->params->status & 1)) {
            FxWeaponAuraDraw(u);
        }
    }
}
