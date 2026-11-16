#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 SndStrmSkip(u32 arg0, s32 arg1) {
    s32 v;

    if (arg0 >= 4) {
        return -2;
    }
    v = sndIopStatus->strm[arg0].secNum;
    if (v < arg1) {
        arg1 = v;
    }
    SndIopCmdPush(0x110, arg0, arg1, 0);
    return arg1;
}
