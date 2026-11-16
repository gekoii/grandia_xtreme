#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

extern u8 dbgChrStrNeckX[];
extern u8 dbgChrStrNeckY[];
extern u8 dbgChrStrStatus[];
extern u8 dbgChrStrColSt[];
extern u8 dbgChrStrAttr[];
extern u8 dbgChrStrAttr2[];
extern u8 dbgChrStrScnrId[];
extern u8 dbgChrStrCrSpeed[];
extern u8 dbgChrStrR[];
extern u8 dbgChrStrDistance[];
extern u8 dbgChrStrSerch[];
extern u8 dbgChrStrDirect[];
extern u8 dbgChrStrTarget[];
extern u8 dbgChrStrJump[];
extern u8 dbgChrStrObj[];
extern u8 dbgChrStrPosX[];
extern u8 dbgChrStrVecY[];
extern u8 dbgChrStrVecZ[];
extern u8 dbgChrStrVectorX[];
extern u8 dbgChrStrTargetX[];
extern u8 dbgChrStrAngleX[];
extern u8 dbgChrStrAngleY[];
extern u8 dbgChrStrAngleZ[];
extern u8 dbgChrStrAiNo[];
extern u8 dbgChrStrAiFrame[];
extern u8 dbgChrStrPartsNum[];

void DbgChrStatusPage(ChrObj *obj, u16 held) {
    s32 ang[3];
    GphPart *n = NULL;
    GphModel *m;
    f32 a;
    f32 b;

    if (obj == NULL) {
        return;
    }
    if (obj->model != NULL) {
        n = ChrModelFindPart(obj->model, 0x280);
    }
    if (n != NULL) {
        b = n->rot[0] * (360.0f / 65536.0f);
        a = n->rot[1] * (360.0f / 65536.0f);
    } else {
        b = a = 0.0f;
    }
    DbgPrintf(4, 5, dbgChrStrNeckX, -b);
    DbgPrintf(4, 6, dbgChrStrNeckY, a);
    DbgPrintf(4, 7, dbgChrStrStatus, obj->aiMode);
    DbgPrintf(4, 8, dbgChrStrColSt, obj->collMask);
    DbgPrintf(4, 9, dbgChrStrAttr, obj->flags);
    DbgPrintf(4, 10, dbgChrStrAttr2, obj->actFlags);
    DbgPrintf(4, 11, dbgChrStrScnrId, obj->id);
    DbgPrintf(4, 12, dbgChrStrCrSpeed, obj->speed);
    DbgPrintf(4, 13, dbgChrStrR, obj->radius, obj->unk38);
    DbgPrintf(4, 14, dbgChrStrDistance, obj->leaderDist);
    if (obj->sense != NULL) {
        DbgPrintf(4, 15, dbgChrStrSerch, obj->sense->range);
    }
    DbgPrintf(4, 16, dbgChrStrDirect, obj->heading);
    DbgPrintf(4, 17, dbgChrStrTarget, obj->moveDir);
    DbgPrintf(4, 18, dbgChrStrJump, obj->headingDeg);
    DbgPrintf(4, 19, dbgChrStrObj, obj->model);
    DbgPrintf(0x1C, 5, dbgChrStrPosX, obj->model->pos[0]);
    DbgPrintf(0x1C, 6, dbgChrStrVecY, obj->model->pos[1]);
    DbgPrintf(0x1C, 7, dbgChrStrVecZ, obj->model->pos[2]);
    DbgPrintf(0x1C, 8, dbgChrStrVectorX, obj->vel[0]);
    DbgPrintf(0x1C, 9, dbgChrStrVecY, obj->vel[1]);
    DbgPrintf(0x1C, 10, dbgChrStrVecZ, obj->vel[2]);
    DbgPrintf(0x1C, 11, dbgChrStrTargetX, obj->moveTarget[0]);
    DbgPrintf(0x1C, 12, dbgChrStrVecY, obj->moveTarget[1]);
    DbgPrintf(0x1C, 13, dbgChrStrVecZ, obj->moveTarget[2]);
    m = obj->model;
    ang[0] = m->rot[0] * (360.0f / 65536.0f);
    ang[1] = (m->rot[1] + 0x8000) * (360.0f / 65536.0f);
    ang[2] = m->rot[2] * (360.0f / 65536.0f);
    while (ang[0] >= 180) {
        ang[0] -= 360;
    }
    while (ang[0] < -180) {
        ang[0] += 360;
    }
    while (ang[1] >= 180) {
        ang[1] -= 360;
    }
    while (ang[1] < -180) {
        ang[1] += 360;
    }
    while (ang[2] >= 180) {
        ang[2] -= 360;
    }
    while (ang[2] < -180) {
        ang[2] += 360;
    }
    DbgPrintf(0x1C, 14, dbgChrStrAngleX, ang[0]);
    DbgPrintf(0x1C, 15, dbgChrStrAngleY, ang[1]);
    DbgPrintf(0x1C, 16, dbgChrStrAngleZ, ang[2]);
    DbgPrintf(0x1C, 17, dbgChrStrAiNo, obj->opState);
    DbgPrintf(0x1C, 18, dbgChrStrAiFrame, obj->opTimer);
    DbgPrintf(0x1C, 19, dbgChrStrPartsNum, obj->model->partCount);
}
