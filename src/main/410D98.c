#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n01.h"


void PtyUnitInitEnemy(PtyUnit *o, PtyEnemyParams *vec, s32 a2, s32 a3, PtyEnemyData *d) {
    s32 i;
    s32 j;

    o->id = a2;
    o->name = d->name;
    o->icon = d->icon;
    o->size = d->size;
    for (i = 0; i < 2; i++) {
        o->race[i] = d->race[i];
    }
    o->type = d->type;
    o->ipStopDmg = d->ipStopDmg;
    o->bitCsf = d->bitCsf;
    o->ipStopDown = d->ipStopDown;
    o->regMgcIpr = d->regMgcIpr * 0.01f;
    o->regKb = (100 - d->regKb) * 0.01f;
    o->colR = d->colR;
    o->colH = d->colH;
    o->nextExp = 0;
    o->runTime = d->runTime;
    o->restartEquip = 1.0f;
    o->exp = d->exp;
    o->restartGuard = 1.0f;
    o->restartEvade = 1.0f;
    o->level = a3;
    o->exp += PtyEnemyRollStats(vec, a3, d);
    o->hp = vec->maxHp;
    o->mp = vec->maxMp;
    o->sp = vec->maxSp;
    o->status = 0;
    for (j = 0; j < 2; j++) {
        o->expSkl[j] = d->expSkl[j];
    }
    o->angry = 0.0f;
}
