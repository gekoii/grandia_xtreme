#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "battle/misc.h"



void BtlCmdMenuTargetInput(void) {
    BtlCmdMenu *m = &btlCmdMenu;
    Pad *pad = padPort0;
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *units;
    s32 id;

    m->timer++;
    if (m->timer >= 32) {
        m->timer = 0;
    }
    if (m->targetCount > 0) {
        if (m->targetCount <= 16) {
            cur = m->unit;
            if (pad->repeat & 0x9000) {
                if (--m->targetSel < 0) {
                    m->targetSel = m->targetCount - 1;
                }
            } else if (pad->repeat & 0x6000) {
                if (++m->targetSel >= m->targetCount) {
                    m->targetSel = 0;
                }
            }
            units = btlUnits;
            if (cur->target != m->targets[m->targetSel]) {
                SndSePlay3D(0x7D1, 0, 0, NULL);
                id = m->targets[m->targetSel];
                cur->target = id;
                m->unk14 = 3;
                u = &units[id];
                BtlUnitInfoOpen(u, m->hidden[u->id]);
                m->timer = 0;
                m->area.center = &u->pos;
                BtlUnitCollectAreaTargetsAtTarget(cur);
                EnvDimStart(cur, 32, 0.62f);
            }
        }
        IpGaugeSetTargets();
    }
}
