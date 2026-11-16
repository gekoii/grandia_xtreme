#include "common.h"
#include "gph.h"



GphLight *GphLightGet(u32 idx) {
    if (gphLightList == NULL) return 0;
    if (idx >= gphLightList->count) return 0;
    return gphLightList->lights[idx];
}
