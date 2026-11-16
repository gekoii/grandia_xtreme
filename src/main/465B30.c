#include "common.h"
#include "main/n11.h"

void readBufDelete(s32 arg0) {

}

s32 readBufBeginPut(void *arg0, void **arg1) {
    MovReadBuf *b = arg0;
    s32 n;

    n = b->size - b->count;
    if (n != 0) {
        *arg1 = b->data + b->put;
    }
    return n;
}
