#include "common.h"
#include "snd.h"

s32 voBufReset(VoBuf *arg0) {
    arg0->count = 0;
    arg0->write = 0;
    return 0;
}
