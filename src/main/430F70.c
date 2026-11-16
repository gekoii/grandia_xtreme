#include "common.h"
#include "main/n03.h"

void GsPacketBufInit(GsPacketBuf *arg0, u32 arg1, s32 arg2) {
    arg1 >>= 1;
    arg2 = (arg2 + 0xF) & 0xFFFFFFF0;
    arg0->size = arg1;
    arg0->index = 0;
    arg0->buf0 = arg2;
    arg0->buf1 = arg2 + arg1;
    arg0->start = arg2;
    arg0->cur = arg2;
}
