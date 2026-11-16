#include "common.h"
#include "snd.h"
#include "protos/main.h"

s32 voBufGetData(VoBuf *arg0) {
    return voBufIsFull(arg0) ? 0 : arg0->data + arg0->write * 0x118000;
}
