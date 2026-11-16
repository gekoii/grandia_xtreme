#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndIopIsRecvDone(void) {
    if (sndIopRecvBusy != 0) {
        if (sceSifCheckStatRpc(&sndIopRecvData) != 0) {
            return 0;
        }
    }
    return 1;
}
