#include "common.h"
#include "field/n25.h"

void MiniPanelSlideOut(MiniPanel *h, u8 mode) {
    h->moveTime = 10;
    h->startX = h->baseX;
    h->flags |= 5;
    h->startY = h->baseY;
    if (mode == 2) {
        h->goalX = h->startX;
        h->goalY = h->startY + 100;
        return;
    }
    h->flags &= 0xFA;
    h->startX = 0;
    h->startY = 0;
    h->goalX = 0;
    h->goalY = 0;
    h->moveTime = 0;
    h->alpha = 0;
}
