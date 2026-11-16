#include "common.h"

extern void GsMfifoSend(void *, s32);

void GsRenderStatePacketSend(void *arg0) {
    GsMfifoSend(arg0, 9);
}
