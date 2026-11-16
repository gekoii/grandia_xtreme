/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
} S732A0;

s32 _sceMpegSetPicBufSize(S732A0 *arg0, s32 arg1, s32 arg2) {
    arg0->x4 = arg1;
    arg0->x8 = arg2;
    arg0->xC = arg1 >> 4;
    arg0->x10 = arg2 >> 4;
    return 1;
}
