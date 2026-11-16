#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "save.h"
#include "field/n16.h"

extern void MemSet(void *, s32, s32);
extern s32 dbgFldMapJumpNo;
extern s32 dbgFldMapJumpLine;
extern s32 dbgFldMapJumpRepeat;
extern u16 dbgFldMapJumpTable[];
extern u8 dbgMapJumpStrTitle[];
extern u8 dbgMapJumpStrMapNo[];
extern u8 dbgMapJumpStrMainFlag[];
extern u8 dbgMapJumpStrRule[];

void DbgFldMapJumpPage(s32 arg0) {
    char buf[0x20];
    Pad *pad;
    u16 b;
    s32 changed;
    s32 x;

    pad = &padPort1;
    DbgDrawCursor(3, dbgFldMapJumpLine + 5);
    changed = 0;
    if (!(padPort1.held & 2)) {
        b = pad->repeatFast;
        if ((b & 0x1000) && dbgFldMapJumpLine != 0) {
            dbgFldMapJumpLine--;
        }
        if ((b & 0x4000) && dbgFldMapJumpLine <= 0) {
            dbgFldMapJumpLine++;
        }
        if (b & 0x8000) {
            switch (dbgFldMapJumpLine) {
            case 0:
                if (dbgFldMapJumpNo != 0) {
                    dbgFldMapJumpNo--;
                } else {
                    dbgFldMapJumpNo = 0xB8;
                }
                break;
            case 1:
                x = sysSaveData.mainFlag;
                if (pad->held & 1) {
                    x -= 100;
                } else {
                    x -= 1;
                }
                if (x < 0) {
                    x = 0x2E9F;
                }
                sysSaveData.mainFlag = x;
                break;
            }
            changed = 1;
        }
        if (b & 0x2000) {
            switch (dbgFldMapJumpLine) {
            case 0:
                if (dbgFldMapJumpNo < 0xB9) {
                    dbgFldMapJumpNo++;
                } else {
                    dbgFldMapJumpNo = 0;
                }
                break;
            case 1:
                if (pad->held & 1) {
                    sysSaveData.mainFlag += 100;
                } else {
                    sysSaveData.mainFlag += 1;
                }
                break;
            }
            changed = 1;
        }
        if (changed) {
            dbgFldMapJumpRepeat++;
        } else {
            dbgFldMapJumpRepeat = 0;
        }
        if (pad->trigger & 0x20) {
            MemSet(buf, 0, 0x20);
            MapMakeMdtPath(dbgFldMapJumpTable[dbgFldMapJumpNo], buf);
            if (FileGetSizeAligned(buf) > 0) {
                MapJumpRequest(dbgFldMapJumpTable[dbgFldMapJumpNo], 0);
            }
        }
    }
    DbgPrintf(2, 3, dbgMapJumpStrTitle, dbgFldPageColor5Str, dbgFldMapJumpNo, 0xB9, dbgFldPageColor0Str);
    DbgPrintf(4, 5, dbgMapJumpStrMapNo, dbgFldMapJumpTable[dbgFldMapJumpNo], sysSaveData.mapNo);
    DbgPrintf(4, 6, dbgMapJumpStrMainFlag, sysSaveData.mainFlag);
    DbgPrintf(2, 8, dbgMapJumpStrRule);
    AmapDbgStatusPage(AmapGetWork(), 2, 9);
}
