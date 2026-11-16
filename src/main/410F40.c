/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "protos/main.h"
#include "main/n01.h"

#define RND01() ((rand() >> 15) * (1.0f / 65536.0f))
#define RAND_DELTA(hi, lo) ((f32)((hi) - (lo)) * RND01() + 0.5f + (lo))

s32 PtyEnemyRollStats(PtyEnemyParams *out, s32 lv, PtyEnemyData *d) {
    s32 sum;
    s32 v;
    s32 i;
    s32 j;
    s32 cnt;
    s8 n;
    f32 f;
    PtyEnemyGrowth *g = &d->growth;

    sum = 0;
    out->maxHp = d->params[0];
    out->maxMp = d->params[1];
    out->maxSp = d->params[2];
    out->stats[0] = d->params[3];
    out->stats[1] = d->params[4];
    out->stats[2] = d->params[5];
    out->stats[3] = d->params[6];
    out->stats[4] = d->params[7];
    out->stats[5] = d->params[8];
    for (; lv >= 2; lv--) {
        sum += (s32)RAND_DELTA(g->expMax, g->expMin);
        v = RAND_DELTA(g->hpMax, g->hpMin);
        if (lv >= 71) {
            v = v * 3;
        } else if (lv >= 41) {
            v = v * (9.0f - (lv - 40) * 0.2f);
        } else {
            v = lv * 0.2f * v + v;
        }
        out->maxHp += v;
        out->maxMp += (s32)RAND_DELTA(g->gains[0][0], g->gains[0][1]);
        out->maxSp += (s32)RAND_DELTA(g->gains[1][0], g->gains[1][1]);
        out->stats[0] += (s32)RAND_DELTA(g->gains[2][0], g->gains[2][1]);
        out->stats[1] += (s32)RAND_DELTA(g->gains[3][0], g->gains[3][1]);
        out->stats[2] += (s32)RAND_DELTA(g->gains[4][0], g->gains[4][1]);
        out->stats[3] += (s32)RAND_DELTA(g->gains[5][0], g->gains[5][1]);
        out->stats[4] += (s32)RAND_DELTA(g->gains[6][0], g->gains[6][1]);
        out->stats[5] += (s32)RAND_DELTA(g->gains[7][0], g->gains[7][1]);
        if (lv >= 81) {
            out->stats[0] -= 2;
            out->stats[1] -= 2;
        } else if (lv >= 71) {
            out->stats[0] -= 1;
            out->stats[1] -= 1;
        } else if (lv >= 61) {
        } else if (lv >= 51) {
            out->stats[0] += 1;
            out->stats[1] += 1;
        } else if (lv >= 41) {
            out->stats[0] += 2;
            out->stats[1] += 2;
        } else if (lv >= 31) {
            out->stats[0] += 3;
            out->stats[1] += 3;
        } else if (lv >= 21) {
            out->stats[0] += 2;
            out->stats[1] += 2;
        } else if (lv >= 11) {
            out->stats[0] += 1;
            out->stats[1] += 1;
        }
    }
    for (j = 0; j < 2; j++) {
        out->effectTypes[j] = d->effectTypes[j];
        out->effectValues[j] = d->effectValues[j];
    }
    n = 0;
    cnt = ptyEnemySkillCount;
    if (cnt > 0) {
        if (cnt > 6) {
            cnt = 6;
        }
        for (i = 0; i < cnt; i++) {
            PtySkillDef *e;

            f = (d->skillRolls[i] - d->skillRolls[i] % 10) * 0.01f;
            if (d->skills[i] > 0 && d->skillRolls[i] >= 10 && !(f < RND01())) {
                out->skills[n] = d->skills[i];
                out->skillLevels[n] = d->skillRolls[i] % 10;
                e = &ptySkillTable[out->skills[n]];
                if (out->skillLevels[n] == 0) {
                    out->skillLevels[n] = (e->maxLevel * RND01() > 0.0f) ? (s32)(e->maxLevel * RND01()) + 1 : -(s32)(-e->maxLevel * RND01()) + 1;
                }
                if (out->skillLevels[n] > e->maxLevel) {
                    out->skillLevels[n] = e->maxLevel;
                }
                n++;
            }
        }
    }
    for (; n < 6; n++) {
        out->skills[n] = out->skillLevels[n] = 0;
    }
    return sum;
}
