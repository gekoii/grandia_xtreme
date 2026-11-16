/* rodata: 0x004A41A0-0x004A4230 */
#include "common.h"
#include "pad.h"
#include "protos/main.h"


extern char dbgPadStrPadInformationMenu[];
extern char dbgPadStrType[];
extern char dbgPadStrId[];
extern char dbgPadStrData[];
extern char dbgPadStrDataf[];
extern char dbgPadStrDatar2[];
extern char dbgPadStrLeftAnlgH[];
extern char dbgPadStrLeftAnlgV[];
extern char dbgPadStrRightAnlgH[];
extern char dbgPadStrRightAnlgV[];
extern char dbgPadStrRightMotor[];
extern char dbgPadStrLeftMotor[];
extern char dbgPadStrPressModeViewer[];
extern char dbgPadStrIndexValue[];
extern char dbgPadStrCursor[];


void DbgPadPage(void) {
    s32 idx;
    s32 i;
    s32 j;

    dbgPad = &padPort1;
    DbgPuts(2, 3, dbgPadStrPadInformationMenu);
    {
        char names[6][24] = {
            "Unknow PAD TYPE",
            "DEGITAL CONTROLER",
            "DUAL SHOCK 1",
            "DUAL SHOCK 2",
            "NEGI CON",
            "ANALOG JOYSTICK",
        };

        switch (dbgPad->modeId) {
        case 4:
            idx = 1;
            break;
        case 7:
            idx = dbgPad->pressureMode ? 3 : 2;
            break;
        case 2:
            idx = 4;
            break;
        case 5:
            idx = 5;
            break;
        default:
            idx = 0;
            break;
        }
        DbgPrintf(2, 5, dbgPadStrType, names[idx]);
    }
    DbgPrintf(2, 6, dbgPadStrId, dbgPad->modeId);
    DbgPrintf(2, 7, dbgPadStrData, dbgPad->held);
    DbgPrintf(2, 8, dbgPadStrDataf, dbgPad->trigger);
    DbgPrintf(2, 9, dbgPadStrDatar2, dbgPad->repeatFast);
    DbgPrintf(2, 10, dbgPadStrLeftAnlgH, dbgPad->leftX);
    DbgPrintf(2, 11, dbgPadStrLeftAnlgV, dbgPad->leftY);
    DbgPrintf(2, 12, dbgPadStrRightAnlgH, dbgPad->rightX);
    DbgPrintf(2, 13, dbgPadStrRightAnlgV, dbgPad->rightY);
    DbgPrintf(2, 14, dbgPadStrRightMotor, dbgPadMotorRight);
    DbgPrintf(2, 15, dbgPadStrLeftMotor, dbgPadMotorLeft);
    DbgPrintf(2, 16, dbgPadStrPressModeViewer);
    for (i = 0; i < 16; i++) {
        DbgPrintf(0x1C, i + 3, dbgPadStrIndexValue, i, dbgPad->pressure[i]);
        for (j = 0; j < dbgPad->pressure[i] / 10; j++) {
            DbgPrintf(j + 0x24, i + 3, dbgPadStrCursor);
        }
    }
    if (!(dbgPad->held & 2)) {
        if (dbgPad->repeatFast & 0x1000) {
            dbgPadMotorRight = 0;
            PadVibrate((u8 *)dbgPad, 0, dbgPadMotorLeft, 0xFF);
        }
        if (dbgPad->repeatFast & 0x4000) {
            dbgPadMotorRight = 1;
            PadVibrate((u8 *)dbgPad, 1, dbgPadMotorLeft, 0xFF);
        }
        if (dbgPad->repeatFast & 0x8000) {
            dbgPadMotorLeft--;
            PadVibrate((u8 *)dbgPad, dbgPadMotorRight, dbgPadMotorLeft, 0xFF);
        }
        if (dbgPad->repeatFast & 0x2000) {
            dbgPadMotorLeft++;
            PadVibrate((u8 *)dbgPad, dbgPadMotorRight, dbgPadMotorLeft, 0xFF);
        }
    }
}
