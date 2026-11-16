#include "common.h"
#include "protos/field.h"
#include "field/n17.h"


void MapModelAttachEffects(void) {
    s32 i;

    for (i = 0; i < mapModelCount; i++) {
        EffSpawnModelEffects(mapModelTable[i].model);
    }
}
