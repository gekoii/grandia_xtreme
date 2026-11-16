#include "common.h"
#include "field/n15.h"

void CamFollowSetRate(s32 arg0, f32 fparg0) {
    switch (arg0) {                                 /* irregular */
    case 0:
        camFollow->rate[0] = fparg0;
        return;
    case 1:
        camFollow->rate[1] = fparg0;
        return;
    case 2:
        camFollow->rate[2] = fparg0;
        return;
    case 3:
        camFollow->rate[3] = fparg0;
        return;
    }
}
