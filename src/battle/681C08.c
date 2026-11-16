#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void FxHomingPoolFree(FxHomingPool *p) {
    if (p->entries != NULL) {
        MemFree(p->entries);
        p->entries = NULL;
    }
}
