#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

void MenuCursorShow(f32 arg0, f32 arg1) {
    if (menuCursor.mode != 0) {
        MenuCursorMoveTo(arg0, arg1);
        return;
    }
    menuCursor.x = arg0;
    menuCursor.mode = 1;
    menuCursor.y = arg1;
    menuCursor.timer = 0;
}
