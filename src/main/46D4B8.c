/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} Info;

extern Info *sceGsGetGParam(void);

s32 sceGsSyncV(s32 arg0) {
    Info *gp;
    s64 r;

    gp = sceGsGetGParam();
    if (gp->unk8 == 0) {
        _sceWaitVSync();
        if (gp->unk0 == 1) {
            return (*(volatile u64 *)0x12001000 >> 13) & 1;
        }
    } else {
        r = (_sceWaitVSyncCsr() >> 13) & 1;
        if (gp->unk0 == 1) {
            return r;
        }
    }
    return 1;
}
