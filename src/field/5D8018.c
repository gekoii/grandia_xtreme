#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n16.h"

extern u8 dbgChrStrTypeMonster[];
extern u8 dbgChrStrTypeTenant[];
extern u8 dbgChrStrSit[];
extern u8 dbgChrStrWake[];
extern u8 dbgChrStrDisp[];
extern u8 dbgChrStrExist[];
extern u8 dbgChrStrCollision[];
extern u8 dbgChrStrGravity[];
extern u8 dbgChrStrShadow[];
extern u8 dbgChrStrMoveEnable[];
extern u8 dbgChrStrMotionOverwrite[];
extern u8 dbgChrStrAutoDirect[];
extern u8 dbgChrStrTalk[];
extern u8 dbgChrStrTalkAutoDirect[];
extern u8 dbgChrStrTalkJumpMove[];
extern u8 dbgChrStrGroundCollision[];
extern u8 dbgChrStrLineCollision[];
extern u8 dbgChrStrAiExec[];
extern u8 dbgChrStrSetFileFlag[];

#define ONOFF(c) ((c) ? dbgFldOffOnStr[1] : dbgFldOffOnStr[0])

void DbgChrFlagsPage(ChrObj *obj, u16 held) {
    s32 f;

    if (obj != NULL) {
        if (obj->flags & 0x10) {
            DbgPrintf(4, 5, dbgChrStrTypeMonster);
        } else {
            DbgPrintf(4, 5, dbgChrStrTypeTenant);
        }
        if (obj->actFlags & 2) {
            DbgPrintf(0x20, 5, dbgChrStrSit);
        } else {
            DbgPrintf(0x20, 5, dbgChrStrWake);
        }
        if (padPort0[1].trigger & 0x1000) {
            f = (u16)obj->flags;
            if (f & 1) {
                obj->flags = f ^ 1;
                f ^= 1;
            }
        } else {
            f = (u16)obj->flags;
        }
        if (padPort0[1].trigger & 0x4000) {
            obj->flags = f | 1;
            f |= 1;
        }
        DbgPrintf(4, 6, dbgChrStrDisp, dbgFldOffOnStr[f & 1]);
        DbgPrintf(4, 7, dbgChrStrExist, ONOFF(obj->flags & 2));
        DbgPrintf(4, 8, dbgChrStrCollision, ONOFF(obj->flags & 2));
        DbgPrintf(4, 9, dbgChrStrGravity, ONOFF(obj->flags & 4));
        DbgPrintf(4, 10, dbgChrStrShadow, ONOFF(obj->flags & 8));
        DbgPrintf(4, 11, dbgChrStrMoveEnable, ONOFF(obj->flags & 0x20));
        DbgPrintf(4, 12, dbgChrStrMotionOverwrite, ONOFF(obj->flags & 0x40));
        DbgPrintf(4, 13, dbgChrStrAutoDirect, ONOFF(obj->flags & 0x80));
        DbgPrintf(4, 14, dbgChrStrTalk, dbgFldOffOnStr[1]);
        DbgPrintf(4, 15, dbgChrStrTalkAutoDirect, ONOFF(obj->flags & 0x200));
        DbgPrintf(4, 16, dbgChrStrTalkJumpMove, ONOFF(obj->flags & 0x2000));
        DbgPrintf(4, 17, dbgChrStrGroundCollision, ONOFF(obj->flags & 0x400));
        DbgPrintf(4, 18, dbgChrStrLineCollision, ONOFF(obj->flags & 0x800));
        DbgPrintf(4, 19, dbgChrStrAiExec, dbgFldOffOnStr[obj->actFlags & 1]);
        DbgPrintf(4, 20, dbgChrStrSetFileFlag, ((u8 *)obj->areas[0])[2]);
    }
}
