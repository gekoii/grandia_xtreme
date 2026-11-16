/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a;
    s32 b;
} Pair;

void FlushCache(s32);
void __SetSyscall(s32, s32);
void __kCopy(s32, void *, s32);
s32 __GetEntryAddress(s32);
extern u8 _sceKernelAlarmPatch[];
extern u8 _sceKernelAlarmPatchStub[];
extern Pair _sceKernelAlarmSyscallTable[];

void InitAlarm(void) {
    u32 i;
    Pair *p;

    if (!(*(volatile s32 *)0x10001810 & 0x100)) {
        p = _sceKernelAlarmSyscallTable;
        __SetSyscall(p[0].a, p[0].b);
        __kCopy(0x80076000, _sceKernelAlarmPatch, 0x740);
        __kCopy(0x82000, _sceKernelAlarmPatchStub, 0x28);
        FlushCache(0);
        FlushCache(2);
        __SetSyscall(p[1].a, p[1].b);
        for (i = 2; i < 8; i++) {
            __SetSyscall(p[i].a, __GetEntryAddress(p[i].a));
        }
    }
}
