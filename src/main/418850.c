#include "common.h"
#include "main/n02.h"

void MenuCursorSetPos(f32 a, f32 b) {
    menuCursor.targetX = a;
    menuCursor.targetY = b;
    if (menuCursor.mode != 2) {
        menuCursor.x = a;
        menuCursor.y = b;
    }
}
