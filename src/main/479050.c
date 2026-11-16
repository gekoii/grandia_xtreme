/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u64 buf;
    s32 pad8;
    u8 *ptr;
    u32 bits;
    s32 pad14;
    s64 total;
    u8 *start;
    u8 *end;
} BitStream;

void _sceMpegBsFlushBits(BitStream *bs, s32 n) {
    bs->buf <<= n;
    bs->bits -= n;
    while (bs->bits <= 56) {
        bs->buf |= (u64)*bs->ptr++ << (56 - bs->bits);
        if (bs->ptr >= bs->end)
            bs->ptr = bs->start;
        bs->bits += 8;
    }
    bs->total += n;
}
