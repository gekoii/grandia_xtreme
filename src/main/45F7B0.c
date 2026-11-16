#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndStrmGetReady(s32, s32);

s32 SndFsReadSectors(s32 arg0, u8 *buf, s32 offset, s32 count) {
    s32 fd;
    s32 n;
    s32 total;
    s32 i;
    s32 size;
    s32 sectors;

    fd = SndStrmOpen(arg0);
    if (fd < 0) {
        return fd;
    }
    size = sndStrmSlots[fd].size;
    sectors = (size >> 11) + ((size & 0x7FF) != 0);
    if (sectors < count + offset) {
        sndStrmSlots[fd].busy = 0;
        sndStrmFetchSlot = -16;
        sndStrmSlots[fd].size = 0;
        return -4;
    }
    SndStrmSeek(fd, offset);
    n = SndStrmReadStart(fd, count);
    if (n < 0) {
        return n;
    }
    total = 0;
    while (total < count) {
        SndIopPoll();
        for (i = 9999; i >= 0; i--) {
        }
        SndBusyWait();
        n = SndStrmGetReady(fd, 0);
        if (n <= 0) {
            continue;
        }
        if (SndStrmFetch(fd, buf, n) < 0) {
            total = -n;
            break;
        }
        buf += n << 11;
        total += n;
    }
    while (SndStrmClose(fd) != 0) {
        SndIopPoll();
        SndBusyWait();
    }
    SndIopPoll();
    SndBusyWait();
    return total;
}
