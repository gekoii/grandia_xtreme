#include "common.h"
#include "protos/main.h"
#include "main/n01.h"


void SprDrawRectList(SprRectListEntry *p) {
    while (p->rect != NULL) {
        SprDrawRect(p->rect, p->x, p->y);
        p++;
    }
}
