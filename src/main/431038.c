#include "common.h"
#include "main/n03.h"

s32 GsPacketAlloc(s32 arg0) {
    u32 p;
    u32 end;

    p = gsPacketBuf->cur + 0xF;
    p &= ~0xF;
    end = p + arg0;
    if (end >= gsPacketBuf->start + gsPacketBuf->size) {
        return 0;
    }
    gsPacketBuf->cur = end;
    return p;
}
