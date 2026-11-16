#include "common.h"
#include "main/n11.h"

void readBufEndPut(MovReadBuf *arg0, s32 arg1) {
    s32 n;

    n = arg0->size - arg0->count;
    if (arg1 < n) {
        n = arg1;
    }
    arg0->put = (arg0->put + n) % arg0->size;
    arg0->count += n;
}
