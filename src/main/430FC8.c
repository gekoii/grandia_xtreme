#include "common.h"
#include "main/n03.h"

void GsPacketBufSwap(GsPacketBuf *arg0) {
    s32 v;

    arg0->index ^= 1;
    if (arg0->index == 0) {
        v = arg0->buf0;
    } else {
        v = arg0->buf1;
    }
    arg0->cur = v;
    arg0->start = v;
}
