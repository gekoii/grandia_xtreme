#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallAddVelocity(MiniBall *, f32, f32);

void MiniBallStickPush(void) {
    MiniInput *pad;
    MiniBall *c;
    u32 i;
    f32 x;
    f32 y;

    if (miniBallWork->state != 4) return;
    if (miniBallWork->subState != 3) return;
    if (miniBallData->set->pushEnd < miniBallWork->frame) return;
    pad = &miniBallUi->input;
    if (pad->stickWait != 0) return;
    if (MINI_INPUT_STICK(pad) != 1) return;
    for (i = 0; i < 20; i++) {
        c = &miniBallWork->game.balls[i];
        if (c->state == 4) {
            x = pad->pushX * 0.01f * (VuRandSigned(1.0f) * 4.0f + 1.0f);
            y = pad->pushY * 0.01f * (VuRandSigned(1.0f) * 4.0f + 1.0f);
            MiniBallAddVelocity(c, -x, -y);
        }
    }
    miniBallWork->game.tilt[0] -= pad->pushX * 0.0078125f;
    miniBallWork->game.tilt[2] -= pad->pushY * 0.0078125f;
}
