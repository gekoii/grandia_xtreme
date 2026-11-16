#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"

void VuVecCopy(void *, void *);

void BtlUnitCancelAction(BtlUnit *arg0) {
    BtlUnit *u;
    s32 i;

    arg0->flags &= 0xFFFFFF7F;
    BtlSchedActCancel(arg0->id);
    if (arg0->ipMode == 2) {
        arg0->ipPos = arg0->ipRet;
        arg0->flags &= 0xFFFFFF7F;
        arg0->ipMode = 1;
        arg0->ipSpeed = 0;
        arg0->scopeType = 0;
        if (arg0->state >= 0x80) {
            BtlUnitSetState(arg0, 0x80);
        } else {
            arg0->retPhase = 1;
            arg0->retState = 0x80;
        }
        arg0->action = 0x7F;
        VuVecCopy(arg0->moveTarget, arg0->pos);
        arg0->partsChr = -1;
    }
    for (i = 0; i < 3; i++) {
        if (arg0->comboPartners[i] >= 0) {
            u = &btlUnits[arg0->comboPartners[i]];
            u->comboLeader = -1;
            u->flags &= 0xFFFFFF7F;
            arg0->comboPartners[i] = -1;
            if ((u32)(u->action - 6) < 2) {
                u->action = 0x7F;
                if (u->ipMode == 2) {
                    u->ipPos = u->ipRet;
                    u->flags &= 0xFFFFFF7F;
                    u->ipMode = 1;
                    u->ipSpeed = 0;
                    u->scopeType = 0;
                    if (u->state >= 0x80) {
                        BtlUnitSetState(u, 0x80);
                    } else {
                        u->retPhase = 1;
                        u->retState = 0x80;
                    }
                    VuVecCopy(u->moveTarget, u->pos);
                    u->partsChr = -1;
                }
            }
        }
    }
}
