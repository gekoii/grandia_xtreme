/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 size;
    s32 count;
    s32 unk8;
    u8 *ptr;
    u8 buf[1];
} Ring;

void _sceTtyQueuePush(Ring *arg0) {
    arg0->count++;
    arg0->ptr++;
    if (arg0->ptr == arg0->buf + arg0->size) {
        arg0->ptr = arg0->buf;
    }
}
