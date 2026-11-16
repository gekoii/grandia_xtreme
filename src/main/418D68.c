#include "common.h"
#include "main/n02.h"

void MenuLrArrowsSet(f32 x, f32 y, f32 z) {
    menuLrArrows.flashSide = 0;
    menuLrArrows.x = x;
    menuLrArrows.y = y;
    menuLrArrows.halfWidth = z;
}
