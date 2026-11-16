/* as: ee-gcc2.9-991111-01 */
/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the parts[] loop indexes (with it on, it walks a pointer) */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "party.h"

extern s16 btlPartyGetUpSeTable[];

void BtlUnitStateHitRecover(BtlUnit *arg0) {
    PtyUnit *info;
    BtlUnit *u;
    s32 i;
    s32 idx;
    s8 *p;

    if (arg0->phase == 0) {
        switch (arg0->subPhase) {
        case 0:
            arg0->frameSpeed = 1.0f;
            if (arg0->params->status & 0x20E0) {
                if (btlActingUnit < 0xFF) {
                    arg0->flags |= 8;
                }
                BtlUnitSetState(arg0, 0x42);
                return;
            }
            arg0->subPhase = 1;
            arg0->flags &= 0xFFFFFFF7;
        case 1:
            if (arg0->wake > 0 && btlActingUnit == 0xFF) {
                if (--arg0->wake <= 0) {
                    SndSePlay3D(btlPartyGetUpSeTable[arg0->params->id - 0x1001], 0, 8, &arg0->pos[0]);
                }
            }
            if (arg0->frame == arg0->loopStart) {
                if (!(arg0->params->bitCsf & 0x1000)) {
                    arg0->pos[1] = 0.0f;
                }
                info = arg0->params;
                if (arg0->id < 4 && info->angry >= 100.0f && arg0->retState == 0x80) {
                    info->angryProb = 1.0f;
                }
                BtlUnitStateResume(arg0);
                if (arg0->parts[0] >= 0) {
                    p = arg0->parts;
                    for (i = 0; i < 3; i++) {
                        idx = p[i];
                        if (idx >= 0) {
                            u = &btlUnits[idx];
                            if (u->state == 0x7E && !(u->params->status & 0x20E0)) {
                                BtlUnitStateResume(u);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}
