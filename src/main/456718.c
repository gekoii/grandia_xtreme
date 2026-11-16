#include "common.h"

extern void GsMfifoSend(s32, s32);

void GsMiscPacketSend(s32 arg0) {
    GsMfifoSend(arg0, 7);
}
