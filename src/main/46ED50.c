/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"


extern u8 threadGp[];
extern s32 GetThreadId(void);
extern s32 ReferThreadStatus(s32, void *);
extern s32 CreateThread(struct ThreadParam *);
extern s32 StartThread(s32, void *);
extern s32 ChangeThreadPriority(s32, s32);

s32 sceCdInitEeCB(s32 prio, void *stack, s32 size) {
    s32 ret = 1;
    s32 *p = &_sceCdCbThreadId;
    if (*p == 0) {
        _sceCdThreadId = GetThreadId();
        ReferThreadStatus(_sceCdThreadId, &_sceCdThreadStatus);
        _sceCdCbThreadParam.stackSize = size;
        _sceCdCbThreadParam.gpReg = threadGp;
        _sceCdCbThreadParam.entry = _sceCdCallbackThread;
        _sceCdCbThreadParam.stack = stack;
        _sceCdCbThreadParam.initPriority = prio;
        *p = CreateThread(&_sceCdCbThreadParam);
        StartThread(*p, NULL);
    } else {
        ChangeThreadPriority(*p, prio);
        ret = 0;
    }
    return ret;
}
