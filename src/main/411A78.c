#include "common.h"
#include "party.h"
#include "protos/main.h"






#define CLAMP(x, hi) (x) = ((x) < 0) ? 0 : (((x) > (hi)) ? (hi) : (x))

void PtyCharLevelUpStats(s32 idx, s32 level, PtyUnit *bonus) {
    PtyChar *s;
    PtyGrowth *g;
    s32 lv;
    s32 v;

    lv = level - 2;
    s = &ptyCharData[idx];
    g = &ptyGrowthTable[idx];

    v = PtyRollGrowth(lv, s->hpGrowth, g->growth[0], g->growth[1]);
    s->maxHp += v;
    CLAMP(s->maxHp, 99999);
    s->hpGrowth += v;
    CLAMP(s->hpGrowth, 99999);

    v = PtyRollGrowth(lv, s->spGrowth, g->growth[2], g->growth[3]);
    s->maxSp += v;
    CLAMP(s->maxSp, 999);
    s->spGrowth += v;
    CLAMP(s->spGrowth, 999);

    v = PtyRollGrowth(lv, s->statGrowth[0], g->growth[4], g->growth[5]);
    s->stats[0] += v;
    CLAMP(s->stats[0], 9999);
    s->statGrowth[0] += v;
    CLAMP(s->statGrowth[0], 9999);

    v = PtyRollGrowth(lv, s->statGrowth[1], g->growth[6], g->growth[7]);
    s->stats[1] += v;
    CLAMP(s->stats[1], 9999);
    s->statGrowth[1] += v;
    CLAMP(s->statGrowth[1], 9999);

    v = PtyRollGrowth(lv, s->statGrowth[2], g->growth[8], g->growth[9]);
    s->stats[2] += v;
    CLAMP(s->stats[2], 9999);
    s->statGrowth[2] += v;
    CLAMP(s->statGrowth[2], 9999);

    v = PtyRollGrowth(lv, s->statGrowth[3], g->growth[10], g->growth[11]);
    s->stats[3] += v;
    CLAMP(s->stats[3], 9999);
    s->statGrowth[3] += v;
    CLAMP(s->statGrowth[3], 9999);

    v = PtyRollGrowth(lv, s->statGrowth[4], g->growth[12], g->growth[13]);
    s->stats[4] += v;
    CLAMP(s->stats[4], 9999);
    s->statGrowth[4] += v;
    CLAMP(s->statGrowth[4], 9999);

    v = PtyRollGrowth(lv, s->statGrowth[5], g->growth[14], g->growth[15]);
    s->stats[5] += v;
    CLAMP(s->stats[5], 9999);
    s->statGrowth[5] += v;
    CLAMP(s->statGrowth[5], 9999);

    if (bonus != NULL) {
        s->maxHp += bonus->lvupMhp;
        CLAMP(s->maxHp, 99999);
        s->stats[0] += bonus->lvupStats[0];
        CLAMP(s->stats[0], 9999);
        s->stats[1] += bonus->lvupStats[1];
        CLAMP(s->stats[1], 9999);
        s->stats[2] += bonus->lvupStats[2];
        CLAMP(s->stats[2], 9999);
        s->stats[3] += bonus->lvupStats[3];
        CLAMP(s->stats[3], 9999);
        s->stats[4] += bonus->lvupStats[4];
        CLAMP(s->stats[4], 9999);
        s->stats[5] += bonus->lvupStats[5];
        CLAMP(s->stats[5], 9999);
    }
}
