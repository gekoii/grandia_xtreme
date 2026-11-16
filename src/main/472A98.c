/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"

s32 _sceMpegHeapRemain(SceMpegHeap *heap) {
    return (heap->base + heap->size) - heap->cur;
}
