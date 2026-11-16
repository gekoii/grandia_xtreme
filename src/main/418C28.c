#include "common.h"
#include "main/n02.h"

/* Steps 32 entries: each live one speeds up by 0.05 and moves by its speed. */
void MenuSparkleUpdate(void) {
    MenuSparkle *p = menuSparkles;
    s32 active = 0;
    s32 i = 31;
    u8 timer;
    f32 speed;

    do {
        timer = p->timer;
        if (timer != 0) {
            active = 1;
            speed = p->speed + 0.05f;
            p->timer = timer - 2;
            p->speed = speed;
            p->y = p->y + speed;
        }
        i--;
        p++;
    } while (i >= 0);

    if (active == 0) {
        menuSparkleLast = -1;
    }
}
