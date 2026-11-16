#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/camera.h"
#include "party.h"


void BtlUnitStateEnemyPartDie(BtlUnit *arg0) {
    BtlCharDef *r;
    s32 v;
    f32 z;

    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            r = arg0->def;
            if (r->dieParts[0] == 0) {
                v = (arg0->params->id - btlUnits[arg0->body].params->id - 1) * 1000;
                r->dieParts[0] = v + 0x83E;
                r->dieParts[1] = v + 0x8A2;
                r->dieParts2[0] = v + 0x848;
                r->dieParts2[1] = v + 0x8AC;
            }
            arg0->subPhase = 1;
            arg0->stateCount = 0;
        case 1:
            if (arg0->stateCount >= 0x39) {
                arg0->unk2D = 1;
                BtlUnitSetPartHook(arg0, BtlEnemyPartDieModelCb);
                arg0->ambient[2] = 0.0f;
                arg0->ambient[0] = -64.0f;
                arg0->ambient[1] = -128.0f;
                arg0->alphaStep = -0.015625f;
                SndSePlay3D(0x7E1, 0, 8, &arg0->pos[0]);
                FxDefeatBurstStart(arg0);
                arg0->stateCount = 0;
                arg0->subPhase++;
            }
            break;
        case 2:
            if (arg0->stateCount == 0x40) {
                z = 0.0f;
                arg0->unk2D = 2;
                arg0->ambient[2] = z;
                arg0->ambient[1] = z;
                arg0->ambient[0] = z;
                BtlEnemyDrop(arg0, 0);
                BtlDefeatAddExp(arg0);
                arg0->alpha = z;
                arg0->stateCount = 0;
                arg0->subPhase++;
            }
            break;
        case 3:
            if (arg0->stateCount >= 0x40) {
                if (arg0->stateCount == 0x40) {
                    BtlSchedKoDone(arg0->id);
                } else {
                    if (btlEnemyAliveCount > 0 && btlActingUnit == 0xFF && btlCamParam.mode != 1) {
                        BtlCamSetMode(1, 0x80, 0, 0);
                    }
                    BtlUnitSetState(arg0, 0);
                }
            }
            break;
        }
    }
}
