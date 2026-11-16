#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

extern SprRect menuCursorSprite[];

void MenuCursorDraw(void) {
    SprBegin(menuCursor.x, menuCursor.y);
    if (menuCursor.mode == 1) {
        SprDrawRect(menuCursorSprite, -32.0f - (f32)(menuCursor.timer & 0xF), -4);
    } else {
        SprDrawRect(menuCursorSprite, -0x20, -4);
    }
    SprFlush();
}
