#include "common.h"
#include "protos/main.h"
#include "snd.h"


s32 mpegError(void) {
    return 1;
}

s32 mpegNodata(void) {
    videoDecProceedAudio();
    switchThread();
    viBufAddDMA(&movVideoDecViBuf);
    return 1;
}

s32 mpegStopDMA(void) {
    viBufStopDMA(&movVideoDecViBuf);
    return 1;
}

s32 mpegRestartDMA(void) {
    viBufRestartDMA(&movVideoDecViBuf);
    return 1;
}

s32 mpegBackground(void) {
    s32 var_v0;

    SndIopPoll();
    var_v0 = 0x18F;
    do {
        var_v0 -= 1;
    } while (var_v0 >= 0);
    return 1;
}
