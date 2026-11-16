#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveWaitIdle(void) {
    if (mcSaveStateChanged == 0) {
        if (McIsBusy() == 0) {
            ((s32 (*)(s32))McSaveSetState)(2);
        }
    }
}
