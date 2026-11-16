#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"



void BtlUnitsCheckDefeated(void) {
    BtlUnit *u;
    BtlUnit *t;
    s32 i;
    s32 j;
    s32 flags;
    u16 a;

    flags = 0;
    u = btlUnits;
    for (i = 0; i < 0x10; i++, u++) {
        if (u->state < 5) {
            continue;
        }
        if (u->params->hp <= 0) {
            BtlUnitDefeat(u, 0);
            t = NULL;
            if (i < 4) {
                flags |= 1;
            } else {
                flags |= 2;
            }
            a = u->params->id;
            switch (a & 0xFF) {
            case 0x23:
                t = &btlUnits[u->parts[0]];
                if (t->params->hp > 0) {
                    BtlEnemySpawnFrom(t, u, (a & 0xF00) | 0x22);
                } else {
                    t = NULL;
                }
                break;
            case 0x24:
                t = &btlUnits[u->body];
                if (t->params->hp <= 0) {
                    t = NULL;
                } else {
                    BtlEnemySpawnFrom(t, u, (a & 0xF00) | 0x21);
                }
                break;
            }
            if (t != NULL) {
                BtlUnitDefeat(t, 1);
            }
        } else {
            if (u->action == 5 && i >= 4) {
                BtlUnitSetState(u, 0);
                flags |= 2;
                btlEnemyAliveCount--;
            }
        }
    }
    if (flags != 0) {
        u = btlUnits;
        for (i = 0; i < 4; i++, u++) {
            if (u->state >= 0x40 && btlUnits[u->target].state < 5) {
                if (u->target < 4) {
                    if ((u->action != 2 || ptyItemTable[u->skillId].use->unk1 != 5)
                        && ((u32)(u->action - 3) >= 2 || ptyMoveTable[u->skillId].unk1 != 5)) {
                        BtlUnitSelectTarget(u, 0x12, 0x1003);
                    }
                } else {
                    BtlUnitSelectTarget(u, 1, 0x1003);
                }
            }
        }
        for (i = 0; i < 12; i++, u++) {
            if (u->state >= 0x40 && btlUnits[u->target].state < 5) {
                if (u->target < 4) {
                    BtlUnitSelectTarget(u, 1, 0x1003);
                } else {
                    BtlUnitSelectTarget(u, 0x12, 0x1003);
                }
            }
        }
        i = 0;
        u = btlUnits;
        for (; i < 0x10; i++, u++) {
            if (u->state >= 0x40 && u->comboPartners[0] >= 0) {
                for (j = 0; j < 3; j++) {
                    if (u->comboPartners[j] >= 0) {
                        btlUnits[u->comboPartners[j]].target = u->target;
                    }
                }
            }
        }
    }
}
