/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n31.h"


void MgcFallbackSkillHit(MgcWork *t) {
    BtlUnit *o = t->caster;

    switch (t->phase) {
    case 0x100:
        BtlChrSetMotion(o, 4);
        o->loopCount = -1;
        o->loopStart = o->loopEnd;
        t->phase++;
        o->evtLoop = NULL;
    case 0x101:
        BtlUnitTurnToTarget(o);
        if (o->frame == o->loopStart && t->count >= 50) {
            BtlUnitHitAreaTargets(o, 0);
            MgcSetNextProc(MgcFallbackSkillEnd);
            t->phase = 0x11E;
        }
        break;
    }
}
