#include "common.h"
#include "main/n02.h"

void MenuLrArrowsFlash(s8 arg0) {
    MenuLrArrows *p = &menuLrArrows;
    if (arg0 == 1) {
        p->flashSide = arg0;
    } else {
        p->flashSide = 2;
    }
    menuLrArrows.flashTimer = 0x20;
}
