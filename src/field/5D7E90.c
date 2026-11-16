#include "common.h"
#include "protos/field.h"
#include "field/state.h"

extern u8 dbgChrStrMotionId[];
extern u8 dbgChrStrMotionSpeed[];
extern u8 dbgChrStrBorderMax[];
extern u8 dbgChrStrEventAdrs[];
extern u8 dbgChrStrPoint0[];
extern u8 dbgChrStrPoint1[];
extern u8 dbgChrStrPoint2[];
extern u8 dbgChrStrPoint3[];
extern u8 dbgChrStrPoint4[];
extern u8 dbgChrStrPoint5[];
extern u8 dbgChrStrPoint6[];
extern u8 dbgChrStrPoint7[];
extern u8 dbgChrStrFlagId[];

void DbgChrMotionPage(ChrObj *obj, u16 held) {
    if (obj != NULL) {
        DbgPrintf(4, 5, dbgChrStrMotionId, (s8)obj->mot);
        DbgPrintf(4, 6, dbgChrStrMotionSpeed, obj->motSpeed);
        DbgPrintf(4, 7, dbgChrStrBorderMax, obj->wallHitMax * 2);
        DbgPrintf(4, 8, dbgChrStrEventAdrs, obj->moveCmds);
        DbgPrintf(4, 9, dbgChrStrPoint0, obj->areas[0]);
        DbgPrintf(4, 10, dbgChrStrPoint1, obj->areas[1]);
        DbgPrintf(4, 11, dbgChrStrPoint2, obj->areas[2]);
        DbgPrintf(4, 12, dbgChrStrPoint3, obj->areas[3]);
        DbgPrintf(4, 13, dbgChrStrPoint4, obj->areas[4]);
        DbgPrintf(4, 14, dbgChrStrPoint5, obj->areas[5]);
        DbgPrintf(4, 15, dbgChrStrPoint6, obj->areas[6]);
        DbgPrintf(4, 16, dbgChrStrPoint7, obj->areas[7]);
        DbgPrintf(4, 17, dbgChrStrFlagId, obj->unk19);
    }
}
