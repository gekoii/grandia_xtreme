/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} Info;

extern Info *sceGsGetGParam(void);
extern u64 GsPutIMR(u64);
extern s32 DisableIntc(s32);
extern s32 RemoveIntcHandler(s32, s32);
extern void SetGsCrt(s32, s32, s32);

void sceGsResetGraph(s16 mode, s16 inter, s16 omode, s16 ffmode) {
    Info *gp;

    switch (mode) {
    case 0:
        gp = sceGsGetGParam();
        *(volatile u64 *)0x12001000 = 0x200;
        gp->unk0 = inter;
        gp->unk2 = omode;
        gp->unk6 = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        GsPutIMR(0xFF00);
        gp->unk4 = ffmode != 0;
        if (gp->unk8 != 0) {
            DisableIntc(2);
            RemoveIntcHandler(2, gp->unkC);
            gp->unk8 = 0;
            gp->unkC = 0;
        }
        SetGsCrt(inter & 1, omode & 0xFF, ffmode & 1);
        break;
    case 1:
        *(volatile u64 *)0x12001000 = 0x100;
        break;
    case 5:
        gp = sceGsGetGParam();
        gp->unk4 = ffmode != 0;
        gp->unk0 = inter;
        gp->unk2 = omode;
        gp->unk6 = (*(volatile u64 *)0x12001000 >> 16) & 0xFF;
        SetGsCrt(inter & 1, omode & 0xFF, ffmode & 1);
        break;
    }
}
