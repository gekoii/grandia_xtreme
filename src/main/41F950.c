#include "common.h"

struct FldMapSet; /* field/n22.h */

extern struct FldMapSet *mapSetFileCopy;
extern struct FldMapSet *mapSetFile;

void MapSetFileReset(void) {
    mapSetFile = 0;
    mapSetFileCopy = 0;
}

void MapSetFileSet(struct FldMapSet *arg0) {
    mapSetFile = arg0;
    mapSetFileCopy = arg0;
}

struct FldMapSet *MapSetFileGet(void) {
    return mapSetFile;
}

void DbgBattlePreSetup(void) {

}
