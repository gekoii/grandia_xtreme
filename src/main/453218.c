#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphAnimStateInit(GphAnimState *arg0) {
    if (gphCurAnimSet != NULL) {
        if (gphCurAnimSet->flags & 8) {
            arg0->cell = VuRandInt(gphCurAnimSet->cellCount);
            arg0->time = VuRandInt(0x100);
            arg0->key = VuRandInt(gphCurAnimSet->keyCount);
        } else {
            arg0->cell = 0;
            arg0->time = 0.0f;
            arg0->key = 0;
        }
        arg0->rep = 0;
        arg0->done = 0;
    }
}
