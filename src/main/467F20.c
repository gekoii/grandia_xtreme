#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 audioDecReset(MovAudioDec *arg0) {
    SndIopCmdPush(0x228, 0, 0, 0);
    arg0->state = 0;
    arg0->hdrCount = 0;
    arg0->put = 0;
    arg0->count = 0;
    arg0->totalBytes = 0;
    arg0->totalBytesSent = 0;
    arg0->iopLastPos = 0;
    return 0;
}
