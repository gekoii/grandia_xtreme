/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/eekernel.h"

s32 CreateSema(SemaParam *);
void SetAlarm(s32, void (*)(), s32);
void WaitSema(s32);
void DeleteSema(s32);
void _sceCdSleepAlarmHandler();

void _sceCdSleep(u16 arg0) {
    SemaParam param;
    s32 sema;

    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    SetAlarm(arg0, _sceCdSleepAlarmHandler, sema);
    WaitSema(sema);
    DeleteSema(sema);
}
