/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x006E2760-0x006E2818 */
#include "common.h"
#include "battle/n33.h"
#include "party.h"

void BtlHudGaugeAddHit(s32 dmg, BtlUnit *u, s8 n) {
    BtlHudUnitGauge *g;
    PtyUnit *s;
    s32 st;
    s32 hp;

    g = &btlHudUnitGauges[u->id];
    st = g->hits;
    if (st <= 0) {
        s = u->params;
        st = 0;
        g->damage = 0;
        g->scale = 1.0f;
        hp = s->hp;
        g->hpShown = hp;
        g->popIn = 0.0f;
        g->flash = 0.0f;
        g->barPos = (f32)(s->maxHp - hp) / s->maxHp * 76.0f + 4.0f;
        g->hits = 0;
        g->timeout = 0;
    }
    if (dmg >= 0) {
        g->hits = st + n;
        if (g->hits <= 0) {
            g->hits = 1;
        }
        st = g->hits;
        g->damage += dmg;
        g->flash = 1.0f;
        switch (st) {
        case 5:
            g->scale = 1.4f;
            break;
        case 10:
            g->scale = 1.8f;
            break;
        case 20:
            g->scale = 2.3f;
            break;
        case 30:
            g->scale = 2.9f;
            break;
        case 50:
            g->scale = 4.0f;
            break;
        }
    } else {
        g->timeout = 0x60;
    }
    if (st >= 3) {
        g->shake = 12;
    } else {
        g->shake = 0;
    }
}
