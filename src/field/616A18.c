/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void AmapFloorBannerUpdate(void) {
    if (amapState->bannerOn != 0) {
        if (EvtScnrIsRunning() != 1) {
            amapState->bannerT += 0.05f;
            if (amapState->bannerT >= 1.0f) {
                amapState->bannerT = 1.0f;
            }
            if (amapState->bannerT == 1.0f) {
                amapState->bannerAlpha--;
                if (amapState->bannerAlpha < 0x30) {
                    amapState->bannerAlpha = 0x30;
                }
            }
        }
    }
}
