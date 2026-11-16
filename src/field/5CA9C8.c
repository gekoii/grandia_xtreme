#include "common.h"
#include "field/n15.h"

f32 CamFollowGetRate(s32 i) {
    f32 r = 0.0f;

    switch (i) {
    case 0:
        r = camFollow->rate[0];
        break;
    case 1:
        r = camFollow->rate[1];
        break;
    case 2:
        r = camFollow->rate[2];
        break;
    case 3:
        r = camFollow->rate[3];
        break;
    }
    return r;
}
