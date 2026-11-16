#include "common.h"
#include "snd.h"

s32 voBufIsEmpty(void *arg0) {
    VoBuf *f = arg0;

    return f->count == 0;
}
