/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

M2C_UNK SetAlarm(s32, M2C_UNK *, s32);         /* extern */
s32 GetThreadId();                                /* extern */
M2C_UNK SleepThread();                            /* extern */
extern M2C_UNK _sceMcSleepAlarmHandler;

void _sceMcSleep(s32 arg0) {
    SetAlarm(arg0 & 0xFFFF, &_sceMcSleepAlarmHandler, GetThreadId());
    SleepThread();
}
