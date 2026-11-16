#include "common.h"
#include "battle/misc.h"

void MgcInit(void) {
    mgcWork.model = 0;
    mgcWork.update = 0;
    mgcWork.state = 0;
    mgcWork.draw = 0;
    mgcWork.drawList = 0;
    mgcWork.textures = 0;
    mgcAbortStep = 0;
}
