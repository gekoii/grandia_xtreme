#include "common.h"

void GsMfifoSend(u16 *, s32);

void GsGifPacketSend(u16 *arg0) {
    GsMfifoSend(arg0, *arg0 + 1);
}
