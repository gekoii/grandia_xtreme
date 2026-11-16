#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

void VuVecCopy(void *, void *);

void BtlEnemySpawnFrom(BtlUnit *arg0, BtlUnit *src, s32 id) {
    s32 n;
    BtlUnit *u;
    PtyUnit *ms;
    PtyUnit *s;
    BtlChrData *e;
    s32 i;

    if (arg0->state < 0x40) {
        return;
    }
    n = BtlEnemyActivate(id, 4);
    if (n >= 0) {
        u = &btlUnits[n];
        ms = arg0->params;
        s = u->params;
        e = u->def->data;
        BtlUnitFadeTo(u, 1, 0, 1.0f);
        VuVecCopy(u->pos, src->pos);
        u->pos[1] = 0;
        VuVecCopy(u->moveTarget, u->pos);
        u->rot[1] = src->rot[1];
        IpIconSetPos(n, IpIconGetPos(arg0->id));
        for (i = 0; i < 2; i++) {
            ptyBattleParams[n - 4].effectTypes[i] = ptyBattleParams[arg0->id - 4].effectTypes[i];
            ptyBattleParams[n - 4].effectValues[i] = ptyBattleParams[arg0->id - 4].effectValues[i];
        }
        s->hp = 0;
        s->sp = 0;
        s->mp = 0;
        s->hp += ms->hp;
        s->hp = s->hp < 0 ? 0 : (s->hp > s->maxHp ? s->maxHp : s->hp);
        s->mp += ms->mp;
        s->mp = s->mp < 0 ? 0 : (s->mp > s->maxMp ? s->maxMp : s->mp);
        s->sp += ms->sp;
        s->sp = s->sp < 0 ? 0 : (s->sp > s->maxSp ? s->maxSp : s->sp);
        s->level = ms->level;
        s->exp = ms->exp;
        for (i = 0; i < 2; i++) {
            s->expSkl[i] = 0;
            s->expSkl[i] = ms->expSkl[i];
            s->expSkl[i] = s->expSkl[i] < e->gaugeMin[i] ? e->gaugeMin[i] : (s->expSkl[i] > e->gaugeMax[i] ? e->gaugeMax[i] : s->expSkl[i]);
        }
        PtyUnitCalcParams(s, e, &ptyBattleParams[n - 4]);
        u->itemRate = (rand() >> 15) * (1.0f / 65536.0f) * 100.0f;
        BtlEnemyDropRoll(u, 0.0f);
    }
    arg0->item = -1;
    arg0->params->exp = 0;
    arg0->treasure = 0;
}
