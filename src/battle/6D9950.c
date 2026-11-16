#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "party.h"
#include "sys.h"



void BtlUnitDefeat(BtlUnit *u, s8 a1) {
    PtyUnit *st;
    BtlUnit *o;
    BtlUnit *w;
    s32 i;
    u16 x;
    s32 lv;
    s32 j;
    s32 id;

    st = u->params;
    BtlSchedKoRequest(u->id);
    st->status = 0x8000;
    u->action = 0x7F;
    u->powUp = 0;
    if (u->flags & 0x40) {
        if (u->id < 4) {
            btlHandoff.unitIds[u->id] = 0;
        }
        BtlUnitSetState(u, 1);
        BtlChrSetMotion(u, 1);
    } else {
        BtlUnitSetState(u, a1 ? 3 : 4);
        BtlChrSetMotion(u, 2);
    }
    BtlHudComboPopupAdd(u->id);
    u->ipMode = 0;
    u->ipPos = 1.0f;
    if (u->id < 4) {
        PtyUnitCalcParams(u->params, 0, 0);
    } else {
        PtyUnitCalcParams(u->params, u->def->data, &ptyBattleParams[u->id - 4]);
    }
    u->loopStart = u->loopEnd;
    u->frameSpeed = 1.0f;
    u->evtLoop = 0;
    if (u->id < 4) {
        btlPartyHurtLevel = 2;
        btlPartyAliveCount--;
        if (!(u->flags & 0x40)) {
            u->frame = 3.0f;
            if ((s8)btlPartyAliveCount <= 0) {
                SndBgmFadeVolume(btlBgmHandle.b, 0x96, 0.0f);
            } else {
                SndBgmFadeVolume(btlBgmHandle.b, 0x96, 60.0f);
            }
            btlSlowMotionRate = 3;
            btlSlowMotionTimer = 0;
        }
    } else {
        if ((s8)--btlEnemyAliveCount <= 0 && a1 == 0) {
            btlVictoryLeader = u->attacker;
            btlUnits[u->attacker].mvp += 0x32;
        } else if (!(u->flag & 0x10000000)) {
            for (j = 0; j < 16; j++) {
                if (btlUnits[j].state > 0) {
                    btlUnits[j].flag |= 0x10000040;
                }
            }
        }
        if (u->body >= 0) {
            o = &btlUnits[u->body];
            for (i = 0; i < 2; i++) {
                if (o->parts[i] == u->id) {
                    break;
                }
            }
            o->flag = 0x40000 << i;
            for (i = 0; i < 3; i++) {
                w = &btlUnits[o->parts[i]];
                if (w->state > 0) {
                    w->flag |= o->flag & 0x1C0000;
                }
            }
        }
        if (a1 == 0) {
            if (btlEncounterFlags & 0x10) {
                if (u->parts[0] >= 0) {
                    btlUnits[u->attacker].mvp += 100;
                } else if (u->body < 0) {
                    btlUnits[u->attacker].mvp += 25;
                } else {
                    btlUnits[u->attacker].mvp += 50;
                }
            } else {
                lv = u->def->data->unk3;
                if (lv >= 5) {
                    btlUnits[u->attacker].mvp += 50;
                } else if (lv >= 3) {
                    btlUnits[u->attacker].mvp += 20;
                } else {
                    btlUnits[u->attacker].mvp += 15;
                }
            }
        }
    }
    if (!(btlEncounterFlags && btlCamParam.mode == 9 && btlCamParam.unitPtr
          && ((x = btlCamParam.unitPtr->params->id & 0xF) == 1 || x == 5))) {
        BtlCamSetMode(9, 0x40, u, 0);
    }
    u->retState = 0x80;
    u->flags = 0;
    if (u->comboPartners[0] >= 0) {
        BtlUnitCancelAction(u);
    } else if (u->comboLeader >= 0) {
        BtlUnitCancelAction(&btlUnits[u->comboLeader]);
    }
}
