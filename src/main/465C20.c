#include "common.h"
#include "protos/main.h"
#include "snd.h"

void sceMpegAddCallback(void *, s32, void *, s32);
void mpegTS(void);

s32 videoDecCreate(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    sceMpegCreate(arg0, arg1, arg2);
    sceMpegAddCallback(arg0, 0, mpegError, 0);
    sceMpegAddCallback(arg0, 1, mpegNodata, 0);
    sceMpegAddCallback(arg0, 2, mpegStopDMA, 0);
    sceMpegAddCallback(arg0, 3, mpegRestartDMA, 0);
    sceMpegAddCallback(arg0, 4, mpegBackground, 0);
    sceMpegAddCallback(arg0, 5, mpegTS, 0);
    videoDecReset(arg0);
    viBufCreate(&((VideoDec *)arg0)->vibuf, arg3, arg4, arg5, arg6, arg7);
    return 1;
}
