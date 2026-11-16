#include "common.h"
#include "main/n11.h"

s32 readBufBeginGet(void *arg0, void **arg1) {
    MovReadBuf *b = arg0;
    s32 n;
    s32 size;

    n = b->count;
    if (n != 0) {
        size = b->size;
        *arg1 = b->data + (b->put - n + size) % size;
    }
    return n;
}
