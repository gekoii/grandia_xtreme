#include "common.h"
#include "protos/main.h"
#include "main/n01.h"


void SprDrawRectFastList(SprRectListEntry *arg0) {
    while (arg0->rect != NULL) {
        SprDrawRectFast(arg0->rect, arg0->x, arg0->y);
        arg0++;
    }
}
