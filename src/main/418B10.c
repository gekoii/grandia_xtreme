#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

void MenuSparkleSpawn(f32 x, f32 y) {
    MenuSparkle *p;
    s32 r;

    menuSparkleLast++;
    if (menuSparkleLast >= 0x20) {
        menuSparkleLast = 0;
    }
    p = &menuSparkles[menuSparkleLast];
    p->x = (x - 24.0f) + ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 24.0f;
    r = rand();
    p->speed = -0.4f;
    p->y = (y + 6.0f) + ((r >> 15) * (1.0f / 65536.0f) - 0.5f) * 24.0f;
    p->angle = rand() >> 15;
    p->timer = 0xA0;
}
