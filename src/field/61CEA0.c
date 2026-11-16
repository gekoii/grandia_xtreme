#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void FldMonsterSpawnAll(void) {
    FldMapSet *p;
    s32 i;

    if (fldCharDefTable == NULL) {
        return;
    }
    p = MapSetFileGet();
    if (p == NULL) {
        return;
    }
    for (i = 0; i < p->monsterCount; i++) {
        FldMonsterSpawn(p, 1);
    }
}
