#include "common.h"
#include "gph.h"

extern void GsMfifoSend(void *, s32);

void GsTexPacketSend(GsTexPacket *a0) {
    GsMfifoSend(a0, 9);
}
