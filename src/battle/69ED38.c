/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"

#include "battle/misc.h"
#include "battle/unit.h"
#include "party.h"

s32 BtlCmdMenuTargetListBuild(s8 mode, s8 arg1) {
    BtlCmdMenu *m = &btlCmdMenu;
    BtlUnit *u;
    s8 *p;
    s8 *q;
    s8 *r;
    s8 *t;
    s8 *v;
    s32 start;
    s32 end;
    s32 i;
    s32 j;
    s32 k;
    s8 a;
    f32 best;
    f32 dx;
    f32 dz;
    f32 d;

    switch (mode) {
    case 1:
    case 5:
        start = 0;
        end = 4;
        break;
    case 2:
        start = 4;
        end = 16;
        break;
    case 6:
        start = 0;
        end = 16;
        break;
    case 3:
        m->targetSel = 0;
        m->targetCount = 1;
        m->targets[0] = m->targetsUnsorted[0] = m->unit->target = m->unit->id;
        return 1;
    default:
        if (arg1 == 2 || arg1 == 4) {
            m->targetCount = 0x12;
            BtlUnitSelectTarget(m->unit, 1, 0x1003);
        } else {
            m->targetCount = 0x11;
            m->unit->target = m->unit->id;
        }
        return 1;
    }
    u = &btlUnits[start];
    p = m->targets;
    q = m->targetsUnsorted;
    m->targetCount = 0;
    for (; start < end; start++, u++) {
        if (!((mode == 5 && u->state >= 4) || (mode != 5 && u->state >= 0x40))) {
            continue;
        }
        if (u->id != m->unit->id && (u->params->status & 1)) {
            continue;
        }
        if (u->params->bitCsf & 8) {
            continue;
        }
        *p++ = *q++ = u->id;
        m->targetCount++;
    }
    if (m->targetCount <= 0) {
        m->unit->target = m->unit->id;
        SndSePlay3D(0x7D4, 0, 0, NULL);
        return 0;
    }
    for (i = 0, r = m->targets; i < m->targetCount - 1; i++, r++) {
        for (t = r + 1, j = i + 1; j < m->targetCount; j++, t++) {
            a = *r;
            if (btlUnits[a].pos[0] < btlUnits[*t].pos[0]) {
                *r = *t;
                *t = a;
            }
        }
    }
    k = m->targetCount;
    if (mode == 5) {
        for (k = 0, v = m->targets; k < m->targetCount; k++, v++) {
            if (*v == m->unit->target && btlUnits[*v].state == 4) {
                break;
            }
        }
        if (k >= m->targetCount) {
            for (k = 0, v = m->targets; k < m->targetCount; k++, v++) {
                if (btlUnits[*v].state == 4) {
                    break;
                }
            }
        }
    }
    if (k >= m->targetCount) {
        for (k = 0, v = m->targets; k < m->targetCount; k++, v++) {
            if (*v == m->unit->target) {
                break;
            }
        }
    }
    if (k < m->targetCount) {
        m->targetSel = k;
    } else {
        best = 9.99998e9f;
        for (k = 0, v = m->targets; k < m->targetCount; k++, v++) {
            dx = btlUnits[*v].pos[0] - m->unit->pos[0];
            dz = btlUnits[*v].pos[2] - m->unit->pos[2];
            d = dx * dx + dz * dz;
            if (d < best) {
                m->targetSel = k;
                best = d;
            }
        }
    }
    m->unit->target = m->targets[m->targetSel];
    return 1;
}
