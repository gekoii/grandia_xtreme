#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

s32 FldMonsterGetCount(void) {
    FldMapSet *p;

    p = MapSetFileGet();
    if (p == NULL) {
        return 0;
    }
    if (fldCharDefTable == NULL) {
        return 0;
    }
    return p->monsterCount + FldSpotCountMonsters();
}
