#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

void MenuWidgetsDraw(void) {
    if (menuSparkleLast >= 0 || menuCursor.mode != 0) {
        GphTexBind(0xEC);
        if (menuSparkleLast >= 0) {
            MenuSparkleDraw();
        }
        if (menuCursor.mode != 0) {
            MenuCursorDraw();
        }
    }
}
