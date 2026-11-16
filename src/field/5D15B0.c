#include "common.h"
#include "field/n15.h"

void *ChrEnemyWorkAlloc(void) {
    ChrEnemyWorkPool *p = chrEnemyWorkPool;
    s32 n = p->count;
    p->count = n + 1;
    return &p->recs[n];
}
