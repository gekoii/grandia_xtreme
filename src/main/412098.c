#include "common.h"
#include "party.h"
#include "protos/main.h"





#define RAND_DELTA(hi, lo) ((f32)((hi) - (lo)) * ((rand() >> 15) * (1.0f / 65536.0f)) + 0.5f + (lo))
#define CLAMP16(x, max) (x) = (x) < 0 ? 0 : ((x) > (max) ? (max) : (x))
#define CLAMP32(x, max) (x) = (x) > 0 ? ((x) > (max) ? (max) : (x)) : 1

void PtyCharLevelDownStats(s32 idx) {
    PtyChar *s = &ptyCharData[idx];
    PtyGrowth *g = &ptyGrowthTable[idx];
    s32 d;

    d = RAND_DELTA(g->growth[0], g->growth[1]);
    s->maxHp -= d;
    CLAMP32(s->maxHp, 99999);
    s->hpGrowth -= d;
    CLAMP32(s->hpGrowth, 99999);

    d = RAND_DELTA(g->growth[2], g->growth[3]);
    s->maxSp -= d;
    CLAMP16(s->maxSp, 999);
    s->spGrowth -= d;
    CLAMP16(s->spGrowth, 999);

#define ST(i) \
    d = RAND_DELTA(g->growth[4 + i * 2], g->growth[5 + i * 2]); \
    s->stats[i] -= d; \
    CLAMP16(s->stats[i], 9999); \
    s->statGrowth[i] -= d; \
    CLAMP16(s->statGrowth[i], 9999);

    ST(0)
    ST(1)
    ST(2)
    ST(3)
    ST(4)
    ST(5)
}
