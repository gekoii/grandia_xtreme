#include "common.h"
#include "gph.h"

void GphLightModuleInit(void) {

}

s32 GphLightListSize(s32 arg0) {
    return (arg0 << 6) + ((arg0 + 4) * 4) + 0x1B0;
}

void GphLightListSet(GphLightList *arg0) {
    gphLightList = arg0;
    gphRenderCtx->lightBlock = (s32)&arg0->block;
}

GphLightList *GphLightListGetCurrent(void) {
    return gphLightList;
}
