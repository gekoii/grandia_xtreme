#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

void MenuWidgetsUpdate(void) {
    if (menuCursor.mode != 0) {
        MenuCursorUpdate();
    }
    if (menuSparkleLast >= 0) {
        MenuSparkleUpdate();
    }
    if (menuLrArrows.flashSide > 0) {
        if (--menuLrArrows.flashTimer == 0) {
            menuLrArrows.flashSide = 0;
        }
    }
    menuWidgetFrame++;
}
