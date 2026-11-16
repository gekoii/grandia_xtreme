/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} Heap00472A30;

extern char _sceMpegWorkAreaSizeStr[];

u32 _sceMpegHeapAlloc(Heap00472A30 *arg0, s32 arg1, u32 arg2) {
    u32 start = (arg0->unk8 + arg2 - 1) / arg2 * arg2;
    u32 end = start + arg1;
    if (arg0->unk0 + arg0->unk4 >= end) {
        arg0->unk8 = end;
        return start;
    }
    ((void (*)(char *, s32))_sceMpegError)(_sceMpegWorkAreaSizeStr, arg1);
    return 0;
}
