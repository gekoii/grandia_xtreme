#include "common.h"
#include "protos/main.h"
#include "snd.h"


s32 videoDecReset(void *arg0) {
    ((VideoDec *)arg0)->state = 0;
    return 0;
}

s32 videoDecDelete(s32 arg0) {
    viBufDelete(&((VideoDec *)arg0)->vibuf);
    sceMpegDelete(arg0);
    return 1;
}

s32 videoDecAbort(void *arg0) {
    ((VideoDec *)arg0)->state = 1;
    return 0;
}

s32 videoDecGetState(void *arg0) {
    return ((VideoDec *)arg0)->state;
}

s32 videoDecSetState(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = ((VideoDec *)arg0)->state;
    ((VideoDec *)arg0)->state = arg1;
    return temp_v0;
}
