#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndSetFade(SndFadeReq *arg0) {
    SndHandle *ch;
    u32 a;

    ch = SndFindHandle(arg0->handle);
    if (ch == NULL) {
        return -3;
    }
    if (ch->state == 0) {
        return -3;
    }
    a = arg0->volume;
    if (a >= 0x100 || (u32)arg0->speed >= 0x100) {
        return -9;
    }
    ch->targetVol = sndVolumeCurve[a ^ 0xFF];
    ch->fadeSpeed = arg0->speed;
    return 0;
}
