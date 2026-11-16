#include "common.h"
#include "main/n03.h"

u32 GsPacketAlloc128(s32 arg0) {
    u32 p;
    u32 end;

    p = gsPacketBuf->cur + 0x7F;
    p &= ~0x7F;
    end = p + arg0;
    if (end >= gsPacketBuf->start + gsPacketBuf->size) {
        return 0;
    }
    gsPacketBuf->cur = end;
    return p;
}
