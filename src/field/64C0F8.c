/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertRewardFlagsRoll(u8 arg0) {
    u8 i;

    for (i = 0; i < 8; i++) {
        if (VuRandSigned(1.0f) + 0.5f < miniConcertData->unkC[arg0 * 8 + i]) {
            if (EvtGlobalFlagGet(i + 0x518) != 1) {
                if (EvtGlobalFlagGet(i + 0x508) == 1) {
                    EvtGlobalFlagSet(i + 0x4F8, 1);
                } else {
                    EvtGlobalFlagSet(i + 0x4E8, 1);
                }
            }
        }
    }
}
