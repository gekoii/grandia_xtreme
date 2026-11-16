#include "common.h"
#include "field/n25.h"

void MiniSpriteInit(MiniSprite *o, MiniCell *c, u8 arg2, u8 arg3, u8 arg4) {
    o->x = 0;
    o->y = 0;
    o->ofsX = c->ofsX;
    o->sx = 1.0f;
    o->sy = 1.0f;
    o->angle = 0;
    o->ofsY = c->ofsY;
    o->goalX = 0;
    o->goalY = 0;
    o->startX = 0;
    o->startY = 0;
    o->moveCount = 0;
    o->moveTime = 0;
    o->layer = c->layer;
    o->active = c->active;
    o->moveFlags = 0;
    o->fxFlags = c->fxFlags;
    o->cellW = arg2;
    o->cellH = arg3;
    o->cellX = 0;
    o->cellY = 0;
    o->pulseX = 0x62;
    o->pulseY = 0x62;
    o->waveT = 0;
    o->wavePeriod = 0xF;
    o->waveAmpX = -0xF;
    o->waveAmpY = 0xF;
    o->waveX = 0;
    o->waveY = 0;
    o->blinkPeriod = 0x78;
    o->stretchT = 0;
    o->stretchSpeed = 0;
    o->stretchRect = 0;
    o->stretchMode = 0;
    o->alphaScale = 0x80;
    o->rect = c->rect;
    o->topLeft = arg4;
    o->rectIsList = 0;
    o->text = NULL;
    o->textOfsX = 0;
    o->textOfsY = 0;
    o->num = NULL;
    o->numOfsX = 0;
    o->numOfsY = 0;
    o->drawFlags = 0;
    o->r = 0x80;
    o->g = 0x80;
    o->b = 0x80;
    o->a = 0x80;
    o->animNo = 0;
    if (c->rect != 0) {
        o->drawFlags = 1;
    }
    if (c->rect >= 100) {
        o->rectIsList = 1;
        o->icon = 0;
        o->rect -= 100;
    }
}
