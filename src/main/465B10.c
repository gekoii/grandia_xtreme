#include "common.h"
#include "main/n11.h"

s32 readBufCreate(MovReadBuf *arg0) {
    arg0->size = 0x180000;
    arg0->put = 0;
    arg0->count = 0;
    return 0;
}
