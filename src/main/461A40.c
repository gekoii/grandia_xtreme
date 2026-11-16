#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndSetTrackVolume(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SndHandle *p;

    p = SndFindHandle(arg0);
    if (p->type != 4) {
        return -4;
    }
    if (p->state == 0) {
        return -10;
    }
    arg2 &= 0x3FFF;
    arg3 &= 0x3FFF;
    return SndIopCmdPush(0x280, p->chan, arg1, (arg2 << 16) | arg3);
}
