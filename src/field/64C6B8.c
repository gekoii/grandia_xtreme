#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniPanelInit(MiniPanel *h, MiniPanelSrc *src, MiniCell *cells, MiniSprite *objs) {
    u32 i;

    h->unk10 = 0;
    h->moveCount = 0;
    h->x = src->x;
    h->y = src->y;
    h->sx = 1.0f;
    h->sy = 1.0f;
    h->baseX = src->x;
    h->baseY = src->y;
    h->sprites = &objs[src->first];
    h->count = src->last - src->first + 1;
    h->active = 1;
    h->flags = 0;
    h->maxLayer = 0;
    h->tex = src->tex;
    h->alpha = 0x80;
    h->showNumbers = 0;
    h->showIcons = 0;
    MiniPanelSlideIn(h, src->slide);
    for (i = 0; i < h->count; i++) {
        MiniSpriteInit(&objs[src->first + i], &cells[src->first + i], src->cellW, src->cellH, src->topLeft);
        if (h->maxLayer < objs[src->first + i].layer) {
            h->maxLayer = objs[src->first + i].layer;
        }
    }
}
