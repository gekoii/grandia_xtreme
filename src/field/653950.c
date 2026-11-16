#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallAddVelocity(MiniBall *, f32, f32);

void MiniBallScriptedGusts(void) {
    MiniBall *c;
    MiniBallGust *w;
    u32 i;
    f32 x;

    if (miniBallWork->state != 4) return;
    if (miniBallWork->subState != 3) return;
    if (miniBallWork->game.gustIndex == miniBallData->set->gustCount) return;
    if (miniBallWork->game.gustWait != 0) {
        miniBallWork->game.gustWait--;
        return;
    }
    w = &miniBallData->gusts[miniBallWork->game.gustIndex];
    if (miniBallWork->game.gustLeft == 0) {
        miniBallWork->game.gustIndex++;
        if (miniBallWork->game.gustIndex != miniBallData->set->gustCount) {
            w++;
            miniBallWork->game.gustLeft = w->len;
            miniBallWork->game.gustWait = w->start - miniBallWork->frame;
        }
        return;
    }
    for (i = 0; i < 20; i++) {
        c = &miniBallWork->game.balls[i];
        if (c->state == 4) {
            x = (VuRandSigned(1.0f) + 0.5f) * w->x;
            MiniBallAddVelocity(c, x, (VuRandSigned(1.0f) + 0.5f) * w->y);
        }
    }
    miniBallWork->game.gustLeft--;
}
