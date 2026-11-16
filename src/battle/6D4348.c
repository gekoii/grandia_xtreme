/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"


void BtlHudHpBarDraw(BtlUnit *u) {
    SprRectListEntry list[4];
    BtlHudUnitGauge *g;
    SprRectListEntry *p;
    SprRect *r;
    f32 cur;
    f32 tgt;

    g = &btlHudUnitGauges[u->id];
    p = list;
    if (g->hits >= 0) {
        tgt = (f32)(u->params->maxHp - g->hpShown) / u->params->maxHp * 76.0f + 4.0f;
        cur = g->barPos;
    } else {
        cur = (f32)(u->params->maxHp - u->params->hp) / u->params->maxHp * 76.0f + 4.0f;
        tgt = cur;
    }
    r = btlHudHpBarRects;
    p->rect = r;
    p->x = 0;
    p->y = 0;
    p++;
    if (tgt < cur) {
        p->rect = r + 1;
        p->y = 0;
        r[1].u0 = tgt;
        p->x = tgt;
        r[1].w = cur - tgt;
        r[1].u1 = cur + 2.0f;
        p++;
    }
    p->rect = r + 2;
    r[2].u0 = cur;
    p->x = cur;
    p->y = 0;
    p[1].rect = NULL;
    r[2].w = (f32)(r[2].u1 - 1) - cur;
    SprDrawRectList(list);
}
