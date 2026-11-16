/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/eekernel.h"

typedef struct {
    s32 unk0;
    s32 count;
} TtyQueue;

extern s32 _sceKernelHelperThreadId;
extern s32 _sceKernelHelperSema;
extern TtyQueue _sceKernelHelperQueue;
extern u8 _sceKernelHelperStack[];
extern u8 threadGp[];
extern void topThread(void *);
extern s32 CreateSema(SemaParam *);
extern s32 DeleteSema(s32);
extern s32 CreateThread(ThreadParam *);
extern s32 StartThread(s32, void *);
extern s32 GetThreadId(void);
extern s32 ChangeThreadPriority(s32, s32);

s32 InitThread(void) {
    ThreadParam tp;
    SemaParam sp;

    if (_sceKernelHelperThreadId > 0) {
        return -1;
    }
    sp.initCount = 0;
    sp.maxCount = 0xFF;
    _sceKernelHelperSema = CreateSema(&sp);
    if (_sceKernelHelperSema < 0) {
        return -1;
    }
    tp.entry = topThread;
    tp.stack = _sceKernelHelperStack;
    tp.stackSize = 0x400;
    tp.gpReg = threadGp;
    tp.initPriority = 0;
    _sceKernelHelperThreadId = CreateThread(&tp);
    if (_sceKernelHelperThreadId < 0) {
        DeleteSema(_sceKernelHelperSema);
        return -1;
    }
    _sceKernelHelperQueue.unk0 = 0;
    _sceKernelHelperQueue.count = 0;
    StartThread(_sceKernelHelperThreadId, &_sceKernelHelperQueue);
    ChangeThreadPriority(GetThreadId(), 1);
    return _sceKernelHelperThreadId;
}
