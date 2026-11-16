#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndIopIsStatusRecvDone(void) {
    if (sndIopStatusRecvBusy != 0 && sceSifCheckStatRpc(&sndIopStatusRecvData) != 0) {
        return 0;
    }
    return 1;
}
