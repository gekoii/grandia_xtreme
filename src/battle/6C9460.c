/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"

extern s32 btlEquipOldParam;

void VuVecCopy(VuVec3 *, VuVec3 *);

void BtlUnitStateEquip(BtlUnit *u) {
    f32 z;
    PtyUnit *st;
    f32 r;

    switch (u->subPhase) {
    case 0:
        z = 0.0f;
        BtlUnitFadeTo(u, 7, 1, z);
        BtlUnitFadeTo(u, 1, 1, 1.0f);
        u->ipSpeed = z;
        u->ipPos = z;
        BtlCaptionShow(ptyItemTable[u->skillId].name, 0x78, u);
        BtlCamSetMode(0, 0, u, BtlCamEquipCb);
        BtlChrSetMotion(u, 7);
        u->subPhase++;
    case 1:
        if (u->loopStart > 0.0f) {
            st = u->params;
            btlEquipOldParam = st->status2;
            PtyUnitCalcParams(st, 0, 0);
            BtlUnitWeaponFxStart(u);
            u->loopCount = -1;
            u->subPhase++;
        }
        break;
    case 2:
        if (u->frame == u->loopStart) {
            BtlChrSetMotion(u, 0);
            VuVecCopy(&u->moveTarget, &u->pos);
            r = u->params->restartEquip;
            u->ipSpeed = 0;
            u->ipRet = r;
            u->scopeType = 0;
            u->ipPos = r;
            u->flags &= 0xFFFFFF7F;
            u->ipMode = 1;
            BtlUnitSetState(u, 0x80);
            BtlUnitFadeTo(u, 7, 0x10, 1.0f);
            BtlSchedActDone(u->id);
            BtlCamSetMode(1, 0x40, NULL, NULL);
            BtlUnitUpdateAutoStatus(u, btlEquipOldParam);
        }
        break;
    }
}
