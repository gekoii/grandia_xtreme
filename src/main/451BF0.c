#include "common.h"
#include "protos/main.h"
#include "main/n07.h"


void GphWipeUpdate(void) {
    s32 v;

    if (gphWipe.active == 0) {
        return;
    }
    gphWipeSprites[0].type = 0;
    if ((u32)(v = gphWipe.type) >= 7) {
        gphWipe.active = 0;
        return;
    }
    switch (v) {
    case 0:
        GphWipeSlide(&gphWipe);
        break;
    case 2:
        GphWipeShutter(&gphWipe);
        break;
    }
    if (gphWipe.state != 2) {
        gphWipe.frame++;
    }
    GphBatchDrawSprites(gphWipeSprites);
}
