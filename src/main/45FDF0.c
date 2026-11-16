#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 SndStrmReadStart(u32 arg0, s32 arg1) {
    s32 *p;
    s32 cur;

    if (arg0 >= 4) {
        return -2;
    }
    if (arg1 <= 0) {
        return -4;
    }
    if (sndStrmSlots[arg0].busy == 0) {
        return -2;
    }
    p = &sndStrmSlots[arg0].pos;
    cur = *p;
    if (sndStrmSlots[arg0].size + 0x800 < (cur + arg1) << 11) {
        return -4;
    }
    if (sndIopStatus->strm[arg0].stat >= 2) {
        return -7;
    }
    if (SndIopCmdPush(0x101, arg0, sndStrmSlots[arg0].lsn + cur, arg1) == 0) {
        return -6;
    }
    *p += arg1;
    return 0;
}
