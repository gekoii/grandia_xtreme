#include "common.h"
#include "main/n03.h"

void GsSetPacketBuf(s32 arg0) {
    gsPacketBuf = (GsPacketBuf *)arg0;
}

s32 GsGetPacketBuf(void) {
    return (s32)gsPacketBuf;
}
