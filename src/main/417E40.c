#include "common.h"
#include "protos/main.h"
#include "main/n02.h"


void MenuNumberDraw(MenuNumField *arg0, s32 arg1) {
    SprRectListEntry buf[10];
    arg0->sprites = buf;
    MenuNumberLayout(arg0, arg1, arg0->x, arg0->y);
    SprDrawRectList(buf);
}
