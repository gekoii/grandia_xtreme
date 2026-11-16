#include "common.h"
#include "main/n11.h"

s32 readBufEndGet(MovReadBuf *arg0, s32 arg1) {
    s32 n = arg0->count;
    if (arg1 < n) n = arg1;
    arg0->count -= n;
    return n;
}
