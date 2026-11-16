#include "common.h"
#include "snd.h"

s32 voBufIsFull(void *arg0) {
    VoBuf *f = arg0;

    return f->count == f->size;
}
