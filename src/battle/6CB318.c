/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/unit.h"
#include "battle/camera.h"


void BtlUnitStateEnemyDie(BtlUnit *arg0) {
    f32 z;
    s32 n;
    GphPart *p;

    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            arg0->subPhase = 1;
            arg0->frameSpeed = 0.5f;
            arg0->stateCount = 0;
        case 1:
            if (arg0->stateCount >= 33) {
                BtlSchedKoDone(arg0->id);
                arg0->stateCount = 0;
                arg0->subPhase++;
            }
            break;
        case 2:
            BtlUnitShakeOffset(arg0, 8.0f);
            if (arg0->stateCount >= 7) {
                z = 0.0f;
                arg0->subPhase++;
                arg0->unkD0[2] = z;
                arg0->unkD0[1] = z;
                arg0->unkD0[0] = z;
                arg0->stateCount = 0;
                if (arg0->model != NULL) {
                    ChrModelSetGaObjFlag2(arg0->model, 1);
                    ChrModelSetPartAttr(arg0->model, 0xB, 1);
                    arg0->ambient[2] = z;
                    arg0->ambient[0] = -64.0f;
                    arg0->ambient[1] = -128.0f;
                    ChrModelSetPartAttr(arg0->model, 2, 1);
                    ChrModelSetPartAttr(arg0->model, 0xD, 0);
                    ChrModelSetPartAttr(arg0->model, 3, 1);
                    n = arg0->model->partCount;
                    p = arg0->model->parts;
                    for (; n > 0; n--) {
                        p->uvScroll[3] = 8;
                        p->uvScroll[1] = 8;
                        p++;
                    }
                    BtlUnitFadeTo(arg0, 1, 0x28, 0.0f);
                }
                SndSePlay3D(0x7E1, 0, 8, &arg0->pos[0]);
                FxDefeatBurstStart(arg0);
            }
            break;
        case 3:
            if (arg0->stateCount >= 70) {
                BtlUnitSetState(arg0, 0);
            } else if (arg0->stateCount >= 40) {
                arg0->alpha = 0.0f;
                if (arg0->stateCount == 40) {
                    if (btlEnemyAliveCount > 0 && btlActingUnit == 0xFF && btlCamParam.mode != 1) {
                        BtlCamSetMode(1, 0x80, 0, 0);
                    }
                    arg0->ambient[0] = arg0->ambient[1] = arg0->ambient[2] = 0.0f;
                    if (arg0->model != NULL) {
                        ChrModelRestorePartAttr(arg0->model, arg0->def->modelPack);
                    }
                    BtlEnemyDrop(arg0, 0);
                    BtlDefeatAddExp(arg0);
                }
            } else {
                if (arg0->knockback > 0.0f) {
                    BtlUnitMoveDir(arg0, arg0->rot[1] + 0x8000, 3, arg0->knockback);
                    arg0->knockback -= 1.0f;
                }
            }
            break;
        }
    }
}
