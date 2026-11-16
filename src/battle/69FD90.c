/* rodata: 0x006E0DA0-0x006E0DB4 */
#include "common.h"
#include "gph.h"
#include "battle/misc.h"
#include "protos/battle.h"

void BtlCmdMenuAreaShow(u8 mode, FVECTOR *center, s16 dur) {
    BtlCmdMenu *m = &btlCmdMenu;
    BtlCmdAreaReq *r = &m->area;
    GphPart *o;

    switch (mode) {
    case 2:
    case 6:
        btlCmdMenuCircleArea->pos[0] = (*center)[0];
        btlCmdMenuCircleArea->pos[1] = 0.0f;
        btlCmdMenuCircleArea->pos[2] = (*center)[2];
        o = BtlChrFindPart(btlCmdMenuCircleArea, 2);
        if (dur > 0) {
            o->flags &= ~8;
        } else {
            o->flags |= 8;
        }
        break;
    case 3:
    case 4:
        btlCmdMenuCircleArea->pos[0] = m->unit->pos[0];
        btlCmdMenuCircleArea->pos[1] = 0.0f;
        btlCmdMenuCircleArea->pos[2] = m->unit->pos[2];
        o = BtlChrFindPart(btlCmdMenuLineArea, 3);
        if (dur > 0) {
            o->flags &= ~8;
        } else {
            o->flags |= 8;
        }
        o = BtlChrFindPart(btlCmdMenuLineArea, 4);
        if (dur > 0) {
            o->flags &= ~8;
        } else {
            o->flags |= 8;
        }
        break;
    case 5:
        btlCmdMenuCircleArea->pos[0] = btlCmdMenuCircleArea->pos[1] = btlCmdMenuCircleArea->pos[2] = 0.0f;
        break;
    default:
        return;
    }
    r->type = mode;
    r->active = 1;
    r->center = center;
    r->scale = 0;
    r->fade = 0;
    if (dur > 0) {
        r->prevSize = r->size;
        r->size = dur;
    } else {
        r->prevSize = 0;
    }
}
