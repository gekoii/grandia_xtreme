#include "common.h"
#include "field/n25.h"
#include "protos/field.h"

void MiniPanelSlideIn(MiniPanel *h, u8 dir) {
    h->active = 1;
    h->flags |= 3;
    h->moveTime = 10;
    h->goalX = h->baseX;
    h->goalY = h->baseY;
    h->alpha = 0;
    if (dir == 10) {
        dir = VuRandInt(8) + 2;
    }
    switch (dir) {
    case 2:
        h->startX = h->baseX;
        h->startY = h->baseY - 100;
        break;
    case 3:
        h->startX = h->baseX + 100;
        h->startY = h->baseY - 100;
        break;
    case 4:
        h->startX = h->baseX + 100;
        h->startY = h->baseY;
        break;
    case 5:
        h->startX = h->baseX + 100;
        h->startY = h->baseY + 100;
        break;
    case 6:
        h->startX = h->baseX;
        h->startY = h->baseY + 100;
        break;
    case 7:
        h->startX = h->baseX - 100;
        h->startY = h->baseY + 100;
        break;
    case 8:
        h->startX = h->baseX - 100;
        h->startY = h->baseY;
        break;
    case 9:
        h->startX = h->baseX - 100;
        h->startY = h->baseY - 100;
        break;
    case 1:
        h->startY = h->baseY;
        h->startX = h->baseX;
        h->flags &= 0xFE;
        break;
    default:
        h->flags &= 0xFC;
        h->x = h->baseX;
        h->y = h->baseY;
        h->startX = 0;
        h->startY = 0;
        h->goalX = 0;
        h->goalY = 0;
        h->moveTime = 0;
        h->alpha = 0x80;
        break;
    }
}
