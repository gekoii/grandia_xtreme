#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallAddVelocity(MiniBall *, f32, f32);

void MiniBallRandomGusts(void) {
    MiniBall *c;
    u32 i;
    f32 x;
    f32 y;

    if (miniBallWork->state == 4) {
        if (miniBallData->set->pushEnd < miniBallWork->frame) return;
        if (miniBallWork->subState == 4) return;
    }
    if (miniBallWork->game.gustWait != 0) {
        miniBallWork->game.gustWait--;
        return;
    }
    if (miniBallWork->game.gustLeft == 0) {
        miniBallWork->game.gustLeft = VuRandInt(3) + 2;
        miniBallWork->game.gustWait = VuRandInt(12) * 20 + 60;
        return;
    }
    for (i = 0; i < 20; i++) {
        c = &miniBallWork->game.balls[i];
        if (c->state == 4) {
            x = VuRandSigned(1.0f) * miniBallData->params->gustX;
            y = VuRandSigned(1.0f) * miniBallData->params->gustY + miniBallData->params->gustBiasY;
            MiniBallAddVelocity(c, -x, -y);
        }
    }
    miniBallWork->game.gustLeft--;
}
