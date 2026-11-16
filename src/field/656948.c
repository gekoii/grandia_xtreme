#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/state.h"
#include "field/n25.h"


void FldTitleStart(void) {
    s32 i;
    s32 n;

    fldState.mode = FLD_MODE_TRANSITION;
    GsTexLoadPacket(&fldTitleWork->logoTex, sysGrxsysPac->tex150, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    ChrAllDeactivate();
    if (EvtGetMapNo() == 0x1820 || EvtGetMapNo() == 0x1830) {
        if (EvtGetMapNo() == 0x1830) {
            GphFadeSetColor(&fldFades[0], 0.0f, 0.0f, 0.0f, 128.0f);
            GphFadeSetColor(&fldFades[1], 0.0f, 0.0f, 0.0f, 0.0f);
            GphFadeStart(&fldFades[0], 0, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f);
        } else {
            GphFadeSetColor(&fldFades[0], 0.0f, 0.0f, 0.0f, 0.0f);
            GphFadeSetColor(&fldFades[1], 0.0f, 0.0f, 0.0f, 0.0f);
        }
    } else {
        GphFadeSetColor(&fldFades[0], 0.0f, 0.0f, 0.0f, 128.0f);
        GphFadeSetColor(&fldFades[1], 0.0f, 0.0f, 0.0f, 128.0f);
        GphFadeStart(&fldFades[0], 0, 0.0f, 0.0f, 0.0f, 64.0f, 60.0f);
        GphFadeStart(&fldFades[1], 0, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f);
    }
    n = 0;
    fldTitleWork->shotNext = 1;
    fldTitleWork->cursor = 1;
    fldTitleWork->shotKeys = (FVECTOR *)sysGrxsysPac->tex154;
    for (i = 0; i < 8; i++) {
        fldTitleWork->order[i] = -1;
    }
    do {
        i = VuRandInt(8);
        if (fldTitleWork->order[i] == -1) {
            fldTitleWork->order[i] = n;
            n++;
        }
    } while (n != 8);
}
