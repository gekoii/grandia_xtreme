/* Port-only C for iWakeupThread (0x0048D030), which the decompilation still has in assembly. */
#include "common.h"

/* The kernel helper thread's command queue (topThread, InitThread): the
   helper reads at rd, interrupt code appends at wr */
typedef struct {
    u8 type;
    u8 val;
} HelperCmd;

typedef struct {
    /* 0x0 */ s32 rd;
    /* 0x4 */ s32 wr;
    /* 0x8 */ HelperCmd cmds[512];
} HelperQueue;

extern s32 _sceKernelHelperThreadId;
extern s32 _sceKernelHelperSema;
extern HelperQueue _sceKernelHelperQueue;
extern s32 GetThreadId(void);
extern s32 _iWakeupThread(s32);
extern s32 iSignalSema(s32);

s32 iWakeupThread(s32 tid);

/* A thread can't wake itself from interrupt context, so that case is
   queued for the helper thread (command 0, WakeupThread). The asm makes
   kernel call -0x2F, iGetThreadId: the interrupt-context entry of
   GetThreadId (call 0x2F), which the port's kernel has as GetThreadId. */
s32 iWakeupThread(s32 tid) {
    s32 self = GetThreadId();
    s32 i;

    if (self != tid) {
        return _iWakeupThread(tid);
    }
    if ((u32)self >= 0x100 || _sceKernelHelperThreadId == 0) {
        return -1;
    }
    i = _sceKernelHelperQueue.wr & 0x1FF;
    _sceKernelHelperQueue.wr = i + 1;
    _sceKernelHelperQueue.cmds[i].type = 0;
    _sceKernelHelperQueue.cmds[i].val = self;
    iSignalSema(_sceKernelHelperSema);
    return self;
}
