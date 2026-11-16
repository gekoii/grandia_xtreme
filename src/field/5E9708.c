#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"
#include "field/n17.h"

extern void VuMtxStackPush(u32);
extern void VuMtxMul(s32, s32, FMATRIX *);

void FldShadowBuildDrawList(void) {
    s32 i;
    s32 n;
    ChrObj *o;
    s32 v;

    GphDrawListSet(fldPartyLitDrawList);
    GphDrawListClear(0);
    FldShadowSetup(0);
    if (camCtl.aiType != CAM_AI_EYE) {
        i = 0;
        do {
            i++;
            o = ChrGetByNo(i);
            if (o == NULL) {
                continue;
            }
            if (!(o->flags & 1)) {
                continue;
            }
            if (!(o->flags & 8)) {
                continue;
            }
            if (!(o->flags & 2)) {
                continue;
            }
            if ((o->actFlags & 0x100) && o->blinkTimer != 0 && o->blinkTimer % 7 >= 3) {
                continue;
            }
            if (o->model != NULL) {
                VuMtxStackPush(0);
                fldShadowMtx[3][1] = o->model->pos[1] + 4.0f;
                v = VuMtxStackGetTop();
                VuMtxMul(v, v, &fldShadowMtx);
                GphModelDrawSimple((s32)o->model);
                VuMtxStackPop();
            }
        } while (i < 9);
    }
    i = 0;
    n = ChrTenantGetCount();
    if (EvtSystemFlagGet(0x72) == 1) {
        n = 0;
    }
    for (; i < n; i++) {
        o = ChrTenantAt(i);
        if (o == NULL) {
            continue;
        }
        if (!(o->flags & 2)) {
            continue;
        }
        if (!(o->flags & 0x1000)) {
            continue;
        }
        if (!(o->flags & 1)) {
            continue;
        }
        if (!(o->flags & 8)) {
            continue;
        }
        if (!(o->flags & 0x10) && EvtSystemFlagGet(0x72) == 1) {
            continue;
        }
        if ((o->flags & 0x10) && EvtSystemFlagGet(0x73) == 1) {
            continue;
        }
        if (o->model != NULL) {
            VuMtxStackPush(0);
            fldShadowMtx[3][1] = o->model->pos[1] + 4.0f;
            v = VuMtxStackGetTop();
            VuMtxMul(v, v, &fldShadowMtx);
            GphModelDrawSimple((s32)o->model);
            VuMtxStackPop();
        }
    }
    GphDrawListSort();
}
