#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 SndStrmGetReady(s32, s32);

s32 SndFsReadRawSectors(s32 arg0, s32 arg1, s32 arg2) {
    s32 total;
    s32 n;

    sndStrmSlots[0].busy = 1;
    do {
        if (SndIopCmdPush(0x101, 0, arg1, arg2) != 0) break;
        SndIopPoll();
        SndBusyWait();
    } while (1);
    total = 0;
    while (total < arg2) {
        SndIopPoll();
        SndBusyWait();
        n = SndStrmGetReady(0, 0);
        if (n > 0) {
            if (SndStrmFetch(0, arg0, n) < 0) {
                SndStrmClose(0);
                return total;
            }
            arg0 += n << 11;
            total += n;
        }
    }
    SndStrmClose(0);
    SndIopPoll();
    SndBusyWait();
    return total;
}
