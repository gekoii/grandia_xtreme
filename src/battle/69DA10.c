#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "battle/unit.h"
#include "sys.h"


void BtlCmdMenuDrawTargets(void) {
    BtlCmdMenu *m = &btlCmdMenu;
    VuVec4 v;
    BtlUnit *u;
    BtlUnit *u2;
    s32 i;
    f32 s;
    SprState *g;

    if (m->state == 0) {
        return;
    }
    if (m->area.active != 0 || (m->state & 0x40)) {
        GphDrawListSet(btlCmdMenuDrawList);
        GphDrawListDrawAll();
    }
    if (m->state == 0x41) {
        return;
    }
    if (!(m->state & 0x40)) {
        return;
    }
    if (m->targetCount >= 16) {
        return;
    }
    if (m->targetCount <= 0) {
        return;
    }
    GphTexBind(0xE9);
    SprBegin(0.0f, 0.0f);
    for (i = 0; i < m->targetCount; i++) {
        u = &btlUnits[m->targets[i]];
        g = &sprState;
        s = 0.5f;
        if (u->id == m->unit->target) {
            s = 0.75f;
        }
        g->sy = s;
        sprState.sx = s;
        BtlChrGetPartPos(&v, u, 1);
        UtilWorldToScreen(&v, &v);
        g->ox = v.x - 80.0f;
        g->oy = v.y - 32.0f;
        BtlHudHpBarDraw(u);
    }
    SprFlush();
    GphTexBind(0xEA);
    u2 = &btlUnits[m->unit->target];
    BtlChrGetPartPos(&v, u2, 1);
    UtilWorldToScreen(&v, &v);
    SprBegin(v.x - 80.0f - 8.0f, v.y - 32.0f - 4.0f);
    BtlUnitIconDraw(u2, 0, 0, 0x3000);
    SprFlush();
}
