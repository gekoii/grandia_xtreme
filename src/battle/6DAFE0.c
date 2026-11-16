#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "battle/n34.h"
#include "protos/battle.h"

/* Real definition is (u32 c, f32 x, f32 y, f32 z, f32 r, f32 h); declared with
   the int last because EABI loads mixed int/float arguments in declaration order. */

void DbgBtlHitDispDraw(void) {
    s32 i;
    s32 j;
    BtlUnit *u;
    DbgBtlHitMark *e;
    u32 col;

    if (dbgBtlHitDispFlags & DBG_BTL_HIT_FIELD) {
        DbgBtlDrawCylinder(0.0f, 0.0f, 0.0f, 1100.0f, 100.0f, 0x8040);
        DbgBtlDrawCylinder(0.0f, 0.0f, 0.0f, 5.0f, 100.0f, 0x8040);
    }
    if (dbgBtlHitDispFlags & DBG_BTL_HIT_CHAR) {
        for (i = 0; i < 16; i++) {
            u = &btlUnits[i];
            if (u->state > 0) {
                DbgBtlDrawCylinder(u->pos[0], u->pos[1], u->pos[2], u->params->colR, u->params->colH, 0x80000030);
            }
        }
    }
    if (dbgBtlHitDispFlags & DBG_BTL_HIT_ATTACK) {
        col = 0xF0F0F000;
        for (j = 0; j < 16; j++) {
            e = &dbgBtlHitMarks[j];
            if (e->alpha) {
                DbgBtlDrawCylinder(e->x, 0.0f, e->z, e->radius, 100.0f, e->alpha | col);
            }
        }
    }
}
