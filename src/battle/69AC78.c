/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"
#include "battle/camera.h"
#include "sys.h"


void FxDelayedCastUpdate(FxAtk *e) {
    FxDelayedCast *o = e->work;
    PtyMove *b;
    BtlAction *r;

    if (o->unit.pos[1] > 0.0f) {
        o->unit.unk1D8 -= 1.0f;
        o->unit.pos[1] += o->unit.unk1D8;
        if (o->unit.pos[1] <= 0.0f) {
            o->unit.unk1D8 *= -0.5f;
            if (o->unit.unk1D8 < 1.0f) {
                e->flags &= ~0x100;
                o->unit.unk1D8 = 0.0f;
                o->unit.pos[1] = 0.0f;
            } else {
                o->unit.pos[1] = 0.0f - o->unit.pos[1];
            }
        }
        return;
    }
    if (btlPhase != 0x80) {
        return;
    }
    if (btlActingUnit != 0xFF && btlActingUnit != 0xFC) {
        return;
    }
    if (o->state == 0) {
        if (--o->timer > 0) {
            return;
        }
        if (BtlSchedFxActRequest()) {
            e->actRequested = 1;
            e->flags |= 0x100;
            o->state++;
        }
        o->timer = 0;
        return;
    }
    switch (o->state) {
    case 1:
        if (o->timer == 0) {
            o->timer = 1;
            BtlUnitCollectAreaTargets(&o->unit, o->unit.pos);
            SndBgmFadeVolume(btlBgmHandle.b, 0x96, 80.0f);
            if (o->unit.id < 4) {
                switch (o->unit.action) {
                case 2:
                    EnvDimForSkill(&o->unit, ptyMoveTable[ptyItemTable[o->unit.skillId].use->moveId].unkF);
                    BtlCaptionShow((u8 *)ptyItemTable[o->unit.skillId].name, 0xB4, 0);
                    break;
                case 3:
                case 4:
                    b = &ptyMoveTable[o->unit.skillId];
                    EnvDimForSkill(&o->unit, b->unkF);
                    BtlCaptionShow((u8 *)b->name, 0xB4, 0);
                    break;
                }
            } else {
                r = BTL_UNIT_ACT(&o->unit);
                EnvDimForSkill(&o->unit, r->dimKind);
                BtlCaptionShow((u8 *)r->name, 0xB4, 0);
            }
        }
        if (MgcForceEnd()) {
            MemHeapSelect(sysState.battleMagicHeap);
            if (o->unit.id < 4 && o->unit.action == 2) {
                MgcStart(ptyItemTable[o->unit.skillId].use->moveId, 0, 0);
            } else {
                MgcStart(o->unit.skillId, 0, 0);
            }
            MgcSetActorInfo(&o->unit, o->hitData);
            MemHeapSelect(sysState.battleHeap);
            fxDelayedCastStep = 0x101;
            o->state++;
        }
        return;
    case 2:
        if (fxDelayedCastStep != 0x102) {
            return;
        }
        MgcSetMotionTime(0.0f);
        MgcSwitchProc();
        o->timer = 0;
        o->state++;
        return;
    case 3:
        if (fxDelayedCastStep != 0x103) {
            return;
        }
        if (++o->timer <= 0x20) {
            return;
        }
        MgcSwitchProc();
        fxDelayedCastStep++;
        if (o->unit.params->id != 0xD1) {
            o->model = NULL;
        }
        o->state++;
        return;
    case 4:
        if (o->unit.params->id == 0xD1 && fxDelayedCastHide) {
            fxDelayedCastHide = 0;
            o->model = NULL;
        }
        if (fxDelayedCastStep != 0x105) {
            return;
        }
        MgcSwitchProc();
        EnvDimStart(0, 0x20, 1.0f);
        btlCamParam.flags |= 2;
        SndBgmFadeVolume(btlBgmHandle.b, 0x96, 100.0f);
        FxAtkFree(e);
        BtlSchedActDone(0x10);
        return;
    }
}
