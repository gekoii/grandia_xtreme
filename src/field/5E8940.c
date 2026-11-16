#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n12.h"


void ChrPartyBuildDrawList(void) {
    CamCtl *st;
    ChrObj *p;
    s32 i;
    s32 r;

    GphDrawListSet(fldPartyDrawList);
    GphDrawListClear(0);
    st = &camCtl;
    if (st->aiType == CAM_AI_EYE && st->target == ChrGetByNo(1)) {
        return;
    }
    for (i = 0; i < 9; i++) {
        p = ChrGetByNo(i + 1);
        if (p != 0 && (p->flags & 2) && (p->flags & 1) && p->model != 0
            && (!(p->actFlags & 0x100) || p->blinkTimer == 0 || (s32)p->blinkTimer % 7 < 3)) {
            GphModelCalcAndDraw(p->model);
            if (i == 0) {
                r = ItemGetEquipModelId(PtyCharGetEquip(0, 0));
                if (r != 0) {
                    ChrExtraPartParticleSpawn(p, r, 16.0f);
                }
            }
        }
    }
    GphDrawListSort();
    GphDrawListEnvMapCalcUv();
}
