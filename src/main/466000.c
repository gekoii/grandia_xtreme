#include "common.h"
#include "protos/main.h"
#include "snd.h"

s32 sceMpegGetPicture(s32, s32, s32);
extern s32 movStrDecodeError;

s32 decBs0(s32 arg0) {
    s32 ret;
    s32 v;

    ret = 1;
    while (sceMpegIsEnd(arg0) == 0) {
        if (videoDecGetState(arg0) == 1) {
            ret = -1;
            break;
        }
        while ((v = voBufGetData(&movVoBuf)) == 0) {
            switchThread();
        }
        if (sceMpegGetPicture(arg0, v, 0x460) < 0) {
            MovErrMessage(&movStrDecodeError);
        }
        voBufIncCount(&movVoBuf);
        switchThread();
    }
    sceMpegReset(arg0);
    return ret;
}
