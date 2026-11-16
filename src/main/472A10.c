/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"

void _sceMpegHeapMark(SceMpegHeap *heap) {
    heap->mark = heap->cur;
}

void _sceMpegHeapRelease(SceMpegHeap *heap) {
    heap->cur = heap->mark;
}
