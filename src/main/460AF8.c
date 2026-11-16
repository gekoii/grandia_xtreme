#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndIopRecv(s32 arg0, s32 arg1, s32 arg2) {
    s32 r;

    if (arg2 <= 0) {
        return -0xD;
    }
    r = sceSifGetOtherData(&sndIopRecvData, arg0, arg1, arg2, 1);
    if (r == 0) {
        sndIopRecvBusy = 1;
    } else {
        sndIopRecvBusy = 0;
    }
    return r;
}
