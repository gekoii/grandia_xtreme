#include "common.h"
#include "gph.h"

GphLight *GphLightFindById(s32 arg0) {
    u32 i;

    for (i = 0; i < gphLightList->count; i++) {
        if (gphLightList->lights[i]->id == arg0) {
            return gphLightList->lights[i];
        }
    }
    return NULL;
}
