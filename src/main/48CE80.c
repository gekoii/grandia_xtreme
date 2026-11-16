/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct { u8 type; u8 val; } Cmd;
typedef struct { s32 idx; s32 pad; Cmd cmds[512]; } CmdQueue;

extern s32 _sceKernelHelperSema;
extern char _sceKernelHelperErrStr[];
extern s32 WaitSema(s32);
extern void RotateThreadReadyQueue(u8);
extern void WakeupThread(u8);
extern void SuspendThread(u8);

void topThread(CmdQueue *q) {
    s32 i;
    for (;;) {
        WaitSema(_sceKernelHelperSema);
        i = q->idx & 0x1FF;
        q->idx = i + 1;
        switch (q->cmds[i].type) {
        case 0:
            WakeupThread(q->cmds[i].val);
            break;
        case 1:
            RotateThreadReadyQueue(q->cmds[i].val);
            break;
        case 2:
            SuspendThread(q->cmds[i].val);
            break;
        default:
            _sceSioPrintf(_sceKernelHelperErrStr);
            break;
        }
    }
}
