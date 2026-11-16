#include "common.h"
#include "sys.h"



void SprPopTransform(void) {
    sprState.sx = sprSavedState.sx;
    sprState.ox = sprSavedState.ox;
    sprState.oy = sprSavedState.oy;
    sprState.sy = sprSavedState.sy;
    sprState.angle = sprSavedState.angle;
}
