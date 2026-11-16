#include "common.h"
#include "pad.h"
#include "field/n25.h"

void MiniInputUpdate(void) {
    Pad *pad;
    MiniInput *p;
    u8 dir;
    s8 lx;
    s8 ly;

    pad = padPort0;
    p = &miniUi->input;
    p->held = pad->held;
    p->trigger = pad->trigger;
    p->repeat = pad->repeat;
    p->stickMoved = 0;
    p->lx = pad->leftX + 0x80;
    p->ly = pad->leftY + 0x80;
    p->dx = 0;
    p->dy = 0;
    p->stickActive = 0;
    dir = 0xFF;
    if (pad->repeat & 0x8000) {
        p->dx = -1;
    }
    if (pad->repeat & 0x2000) {
        p->dx = 1;
    }
    if (pad->repeat & 0x1000) {
        p->dy = -1;
    }
    if (pad->repeat & 0x4000) {
        p->dy = 1;
    }
    if (p->buttonWait != 0) {
        p->buttonWait--;
    }
    if (p->circleWait != 0) {
        p->circleWait--;
    }
    if (p->triangleWait != 0) {
        p->triangleWait--;
    }
    if (p->crossWait != 0) {
        p->crossWait--;
    }
    if (p->squareWait != 0) {
        p->squareWait--;
    }
    if (pad->leftX < 0x60 || pad->leftX > 0xA0 || pad->leftY > 0xA0 || pad->leftY < 0x60) {
        p->stickMoved = 1;
        p->stickActive = 1;
    }
    if ((pad->trigger & 0x20) && p->circleWait == 0) {
        p->circleWait = 5;
        dir = 0;
    }
    if ((pad->trigger & 0x10) && p->triangleWait == 0) {
        p->triangleWait = 5;
        dir = 1;
    }
    if ((pad->trigger & 0x40) && p->crossWait == 0) {
        p->crossWait = 5;
        dir = 2;
    }
    if ((pad->trigger & 0x80) && p->squareWait == 0) {
        p->squareWait = 5;
        dir = 3;
    }
    if (dir != 0xFF && p->buttonWait == 0) {
        p->button = dir;
        p->buttonWait = 5;
    }
    if (p->stickWait == 0) {
    if (p->stickActive == 1) {
        lx = pad->leftX + 0x80;
        ly = pad->leftY + 0x80;
        if (lx > 0 && p->maxX < lx) {
            p->maxX = lx;
        } else {
            p->maxX = 0;
        }
        if (lx < 0 && lx < p->minX) {
            p->minX = lx;
        } else {
            p->minX = 0;
        }
        if (ly > 0 && p->maxY < ly) {
            p->maxY = ly;
        } else {
            p->maxY = 0;
        }
        if (ly > 0 && ly < p->minY) {
            p->minY = ly;
        } else {
            p->minY = 0;
        }
        if ((p->maxX == 0 && p->minX == 0) || (p->maxY == 0 && p->minY == 0)) {
            p->stickWait = p->stickFrames;
            p->maxX = 0;
            p->maxY = 0;
            p->minX = 0;
            p->minY = 0;
            p->stickActive = 0;
            p->stickFlick = 1;
            return;
        }
        if (p->maxX > 0) {
            p->pushX = p->maxX;
        } else {
            p->pushX = p->minX;
        }
        if (p->maxY > 0) {
            p->pushY = p->maxY;
        } else {
            p->pushY = p->minY;
        }
        p->stickFrames++;
    } else {
        p->stickFlick = 0;
        p->stickFrames = 0;
        p->stickWait = 5;
    }
    } else {
        p->stickWait--;
    }
}
