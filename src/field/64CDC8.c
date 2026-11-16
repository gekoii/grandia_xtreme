#include "common.h"
#include "field/n25.h"

void MiniSpriteSetNumberSize(void *arg0, s8 arg1, s8 arg2) {
    ((MiniSprite *)arg0)->num->w = arg1;
    ((MiniSprite *)arg0)->num->h = arg2;
}
