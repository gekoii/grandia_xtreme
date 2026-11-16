#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} Info;


extern Info *sceGsGetGParam(void);
extern s32 dbgSysCursor;
extern s32 dbgSysLine;
extern u8 dbgSysStrSystem[];
extern u8 dbgSysStrIpAddress[];
extern u8 dbgSysStrSceiLibrary214[];
extern u8 dbgSysStrGa3dLibrary20001004[];
extern u8 dbgSysStrGaSoundLibrary20001004[];
extern u8 dbgSysStrFmtS[];
extern u8 dbgSysStrGsVersion[];
extern u8 dbgSysStrGsVscfunc[];
extern u8 dbgSysStrGsVscid[];
extern u8 dbgSysStrGsIntermode[];
extern u8 dbgSysStrGsOutmode[];
extern u8 dbgSysStrGsFfmode[];
extern u8 dbgSysStrScreenMagH[];
extern u8 dbgSysStrScreenMagV[];
extern u8 dbgSysStrDebugMenuOn[];
extern u8 dbgSysStrDebugMenuOff[];
extern u8 dbgSysStrT0Mode[];
extern u8 dbgSysStrT0Count[];
extern u8 dbgSysStrT0Comp[];
extern u8 dbgSysStrT0Hold[];
extern u8 dbgSysStrT1Mode[];
extern u8 dbgSysStrT1Count[];
extern u8 dbgSysStrT1Comp[];
extern u8 dbgSysStrT1Hold[];
extern u8 dbgSysStrT2Mode[];
extern u8 dbgSysStrT2Count[];
extern u8 dbgSysStrT2Comp[];
extern u8 dbgSysStrT3Mode[];
extern u8 dbgSysStrT3Count[];
extern u8 dbgSysStrT3Comp[];

void DbgSystemPage(void) {
    Info *info;
    s32 t;

    info = sceGsGetGParam();
    dbgPad = &padPort1;
    DbgPrintf(2, 3, dbgSysStrSystem);
    DbgPrintf(3, 5, dbgSysStrIpAddress, SysGetBootPath());
    DbgPrintf(3, 6, dbgSysStrSceiLibrary214);
    DbgPrintf(3, 7, dbgSysStrGa3dLibrary20001004);
    DbgPrintf(3, 8, dbgSysStrGaSoundLibrary20001004);
    DbgPrintf(3, 9, dbgSysStrFmtS, SysGetLibVersion());
    DbgPrintf(3, 11, dbgSysStrGsVersion, info->unk6);
    DbgPrintf(3, 12, dbgSysStrGsVscfunc, info->unk8);
    DbgPrintf(3, 13, dbgSysStrGsVscid, info->unkC);
    DbgPrintf(3, 14, dbgSysStrGsIntermode, info->unk0);
    DbgPrintf(3, 15, dbgSysStrGsOutmode, info->unk2);
    DbgPrintf(3, 16, dbgSysStrGsFfmode, info->unk4);
    DbgPrintf(3, 17, dbgSysStrScreenMagH, GphViewGetMagH(0));
    DbgPrintf(3, 18, dbgSysStrScreenMagV, GphViewGetMagV(0));
    if (sysState.debugMode != 0) {
        DbgPrintf(3, 19, dbgSysStrDebugMenuOn);
    } else {
        DbgPrintf(3, 19, dbgSysStrDebugMenuOff);
    }
    DbgMenuCursorDraw(2, dbgSysCursor + 17);
    if (!(dbgPad->held & 2)) {
        t = DbgMenuLineSelect(dbgSysLine, 2);
        dbgSysLine = t;
        dbgSysCursor = t;
        switch (t) {
        case 0:
            if (dbgPad->repeatFast & 0x8000) {
                GphViewSetMagH(0, GphViewGetMagH(0) - 0.001f);
            }
            if (dbgPad->repeatFast & 0x2000) {
                GphViewSetMagH(0, GphViewGetMagH(0) + 0.001f);
            }
            break;
        case 1:
            if (dbgPad->repeatFast & 0x8000) {
                GphViewSetMagV(0, GphViewGetMagV(0) - 0.001f);
            }
            if (dbgPad->repeatFast & 0x2000) {
                GphViewSetMagV(0, GphViewGetMagV(0) + 0.001f);
            }
            break;
        case 2:
            if (dbgPad->repeatFast & 0x8000) {
                sysState.debugMode = 0;
                sysState.pauseMode = 0;
            }
            if (dbgPad->repeatFast & 0x2000) {
                sysState.debugMode = 1;
            }
            break;
        }
    }
    DbgPrintf(0x23, 6, dbgSysStrT0Mode, *(volatile s32 *)0x10000010);
    DbgPrintf(0x23, 7, dbgSysStrT0Count, *(volatile s32 *)0x10000000);
    DbgPrintf(0x23, 8, dbgSysStrT0Comp, *(volatile s32 *)0x10000020);
    DbgPrintf(0x23, 9, dbgSysStrT0Hold, *(volatile s32 *)0x10000030);
    DbgPrintf(0x23, 10, dbgSysStrT1Mode, *(volatile s32 *)0x10000810);
    DbgPrintf(0x23, 11, dbgSysStrT1Count, *(volatile s32 *)0x10000800);
    DbgPrintf(0x23, 12, dbgSysStrT1Comp, *(volatile s32 *)0x10000820);
    DbgPrintf(0x23, 13, dbgSysStrT1Hold, *(volatile s32 *)0x10000830);
    DbgPrintf(0x23, 14, dbgSysStrT2Mode, *(volatile s32 *)0x10001010);
    DbgPrintf(0x23, 15, dbgSysStrT2Count, *(volatile s32 *)0x10001000);
    DbgPrintf(0x23, 16, dbgSysStrT2Comp, *(volatile s32 *)0x10001020);
    DbgPrintf(0x23, 17, dbgSysStrT3Mode, *(volatile s32 *)0x10001810);
    DbgPrintf(0x23, 18, dbgSysStrT3Count, *(volatile s32 *)0x10001800);
    DbgPrintf(0x23, 19, dbgSysStrT3Comp, *(volatile s32 *)0x10001820);
}
