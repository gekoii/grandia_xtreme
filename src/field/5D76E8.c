#include "common.h"
#include "pad.h"
#include "field/state.h"
#include "protos/field.h"

extern u8 dbgChrStrFacialId[];
extern u8 dbgChrStrFaceMode[];
extern u8 dbgChrStrFaceCounter1[];
extern u8 dbgChrStrFaceCounter2[];
extern u8 dbgChrStrFaceCounter3[];
extern u8 dbgChrStrFaceOrgId[];
extern u8 dbgChrStrFaceSubId0[];
extern u8 dbgChrStrFaceSubId1[];
extern u8 dbgChrStrFaceSubId2[];
extern u8 dbgChrStrFaceSubId3[];
extern u8 dbgChrStrFaceSubId4[];
extern u8 dbgChrStrFaceSubId5[];
extern u8 dbgChrStrFaceSubId6[];

void DbgChrFacePage(ChrObj *obj, u16 held) {
    Pad *pad = &padPort1;

    if (obj != NULL) {
        if (pad->trigger & 0x1000) {
            if (obj->faceAnim.face != 0) {
                obj->faceAnim.face--;
            }
            obj->faceAnim.mode = 0;
        }
        if (pad->trigger & 0x4000) {
            if (obj->faceAnim.face < 8) {
                obj->faceAnim.face++;
            }
            obj->faceAnim.mode = 0;
        }
        DbgPrintf(4, 4, dbgChrStrFacialId, obj->faceAnim.face);
        DbgPrintf(4, 5, dbgChrStrFaceMode, obj->faceAnim.mode);
        DbgPrintf(4, 6, dbgChrStrFaceCounter1, obj->faceAnim.blinkStep);
        DbgPrintf(4, 7, dbgChrStrFaceCounter2, obj->faceAnim.blinkTimer);
        DbgPrintf(4, 8, dbgChrStrFaceCounter3, obj->faceAnim.stepTimer);
        if (obj->faceAnim.faces != NULL) {
            DbgPrintf(4, 9, dbgChrStrFaceOrgId, obj->faceAnim.faces[0]);
            DbgPrintf(4, 10, dbgChrStrFaceSubId0, obj->faceAnim.faces[1]);
            DbgPrintf(4, 11, dbgChrStrFaceSubId1, obj->faceAnim.faces[2]);
            DbgPrintf(4, 12, dbgChrStrFaceSubId2, obj->faceAnim.faces[3]);
            DbgPrintf(4, 13, dbgChrStrFaceSubId3, obj->faceAnim.faces[4]);
            DbgPrintf(4, 14, dbgChrStrFaceSubId4, obj->faceAnim.faces[5]);
            DbgPrintf(4, 15, dbgChrStrFaceSubId5, obj->faceAnim.faces[6]);
            DbgPrintf(4, 16, dbgChrStrFaceSubId6, obj->faceAnim.faces[7]);
        }
    }
}
