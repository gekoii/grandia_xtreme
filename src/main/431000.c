#include "common.h"
#include "main/n03.h"

s32 GsPacketGetFree(void) {
    s32 used = (gsPacketBuf->cur + 0xF) & 0xFFFFFFF0;
    s32 n = gsPacketBuf->start + gsPacketBuf->size - used;
    if (n <= 0) n = 0;
    return n;
}
