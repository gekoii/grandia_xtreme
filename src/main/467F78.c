#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 audioDecDelete(MovAudioDec *ad) {
    return 1;
}

s32 audioDecIsPreset(void *arg0) {
    MovAudioDec *ad = arg0;

    return ad->totalBytesSent >= ad->iopBuffSize;
}

s32 audioDecStart(s32 *arg0) {
    SndIopCmdPush(0x218, 0, 0, 0);
    ((MovAudioDec *)arg0)->state = 2;
    return 0;
}
