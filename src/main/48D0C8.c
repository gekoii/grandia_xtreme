/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 type;
    u8 c;
} Ev;

typedef struct {
    s32 unk0;
    s32 count;
    Ev buf[0x200];
} Queue;

s32 iSignalSema(s32);
extern s32 _sceKernelHelperThreadId;
extern s32 _sceKernelHelperSema;
extern Queue _sceKernelHelperQueue;

s32 iRotateThreadReadyQueue(u32 arg0) {
    s32 i;

    if (arg0 >= 0x80 || _sceKernelHelperThreadId == 0) {
        return -1;
    }
    i = _sceKernelHelperQueue.count & 0x1FF;
    _sceKernelHelperQueue.count = i + 1;
    _sceKernelHelperQueue.buf[i].type = 1;
    _sceKernelHelperQueue.buf[i].c = arg0;
    iSignalSema(_sceKernelHelperSema);
    return arg0;
}
