#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


s32 MiniBallDealBalls(void) {
    MiniSprite *o2 = &miniBallWork->game.sprites[28];
    MiniBallGame *s = &miniBallWork->game;
    MiniSprite *o1 = &miniBallWork->game.sprites[29];
    u32 n;

    if (s->launched == 6) {
        MiniMsgBoxShowLines(s->msgBox, 1);
        o1->active = 1;
        o2->active = 1;
        MiniSpriteSetPos(o1, 0, -300);
        MiniSpriteMoveTo(o1, 0, 0, 12);
        return 1;
    }
    if (miniBallWork->frame % 20 == 0) {
        n = s->launched;
        MiniBallLaunch(&s->balls[n], &s->sprites[n + 1], n % 6);
        s->launched++;
    }
    return 0;
}
