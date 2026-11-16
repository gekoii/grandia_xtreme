/* rodata: 0x006E2080-0x006E23A8 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"


void BtlUnitStateSetHandler(BtlUnit *u) {
    BtlUnit *t;

    switch (u->state) {
    case 2:
        u->stateHandler = BtlUnitStateGameOver;
        return;
    case 1:
        u->stateHandler = BtlUnitStateVanish;
        return;
    case 3:
    case 4:
        if (u->id < 4) {
            u->stateHandler = BtlUnitStatePartyKo;
            return;
        }
        if (u->body >= 0) {
            u->stateHandler = BtlUnitStateEnemyPartDie;
            return;
        }
        u->stateHandler = BtlUnitStateEnemyDie;
        return;
    case 0x40:
        u->stateHandler = BtlUnitStateHitRecover;
        return;
    case 0x41:
        u->stateHandler = BtlUnitStateHitLight;
        return;
    case 0x42:
        u->stateHandler = BtlUnitStateHitHeavy;
        return;
    case 0x43:
        u->stateHandler = BtlUnitStateHitGuard;
        return;
    case 0x44:
        u->stateHandler = BtlUnitStateEvade;
        return;
    case 0x45:
        u->stateHandler = BtlUnitStateHitBlocked;
        return;
    case 0x7E:
        u->stateHandler = BtlUnitStatePartHitWait;
        return;
    case 0x80:
        u->stateHandler = BtlUnitStateWait;
        return;
    case 0x81:
        u->stateHandler = BtlUnitStateCommand;
        return;
    case 0x82:
        u->stateHandler = BtlUnitStateVictoryMember;
        return;
    case 0x83:
        u->stateHandler = BtlUnitStateVictory;
        return;
    case 0x84:
        u->stateHandler = BtlUnitStateDefend;
        return;
    case 0xC0:
        u->stateHandler = BtlUnitStateCast;
        return;
    case 0xC1:
        if (u->id < 4) {
            switch (u->action) {
            case 0:
            case 1:
                u->stateHandler = BtlUnitStateActAttack;
                return;
            case 5:
                u->stateHandler = BtlUnitStateEscape;
                return;
            case 4:
                if (u->mgcProg == NULL) {
                    u->mgcProg = btlMgcTagUnknown;
                }
                break;
            }
            u->stateHandler = BtlUnitStateActMgc;
            return;
        } else {
            if (u->partsChr >= 0) {
                t = &btlUnits[u->partsChr];
            } else {
                t = u;
            }
            if (t->skillId == 0) {
                switch ((s8)t->def->type) {
                case 2:
                case 3:
                case 4:
                    u->stateHandler = BtlUnitStateActRush;
                    return;
                case 5:
                    u->stateHandler = BtlUnitStateActCharge;
                    return;
                case 1:
                case 6:
                    u->stateHandler = BtlUnitStateActMultiHit;
                    return;
                default:
                    u->stateHandler = BtlUnitStateActAttack;
                    return;
                }
            }
            if (t->skillId >= 0x5B && t->mgcProg == NULL) {
                t->mgcProg = btlMgcTagUnknown;
            }
        }
        u->stateHandler = BtlUnitStateActMgc;
        return;
    case 0x86:
        u->stateHandler = BtlUnitStateEquip;
        return;
    case 0x85:
        u->stateHandler = BtlUnitStateEscapeFollow;
        return;
    default:
        u->stateHandler = BtlUnitStateNone;
        return;
    }
}
