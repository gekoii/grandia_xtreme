/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 size;
    s32 count;
    u8 *ptr;
    s32 pad;
    u8 buf[1];
} Ring;

void _sceTtyQueuePop(Ring *arg0) {
    arg0->count--;
    arg0->ptr++;
    if (arg0->ptr == arg0->buf + arg0->size) {
        arg0->ptr = arg0->buf;
    }
}
