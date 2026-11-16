/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"
#include "field/n16.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

typedef struct {
    s32 id;
    s32 z;
    u8 c0[4];
    s16 x0;
    s16 y0;
    u8 c1[4];
    s16 x1;
    s16 y1;
    u8 pad[8];
} Ent;


extern char dbgFldCharaMoveNames[][9];
extern f32 dbgFldMoveTargetPos[4];
extern f32 dbgFldMoveDirect;
extern u16 dbgFldMoveSpeed;
extern u8 dbgFldMoveKind;
extern u8 dbgFldMoveIndex;
extern f32 dbgFldMoveNeckX;
extern f32 dbgFldMoveNeckY;
extern f32 dbgFldMoveNeckZ;
extern u8 dbgFldMoveCenterLine;
extern s32 dbgFldMoveLine;
extern char dbgCharaMoveStrTitle[];
extern char dbgCharaMoveStrKindParty[];
extern char dbgCharaMoveStrKindTenant[];
extern char dbgCharaMoveStrPosX[];
extern char dbgCharaMoveStrPosY[];
extern char dbgCharaMoveStrPosZ[];
extern char dbgCharaMoveStrDirect[];
extern char dbgCharaMoveStrSpeed[];
extern char dbgCharaMoveStrNeckX[];
extern char dbgCharaMoveStrNeckY[];
extern char dbgCharaMoveStrNeckZ[];
extern char dbgCharaMoveStrMove[];
extern char dbgCharaMoveStrSetCursor[];
extern char dbgCharaMoveStrCenterLine[];

void DbgFldCharaMovePage(void) {
    Pad *pad = &padPort1;
    ChrObj *obj;
    GphPart *node;
    Ent e[3];
    Ent *p;

    DbgPrintf(4, 4, dbgCharaMoveStrTitle);
    if (dbgFldMoveKind == 0) {
        obj = ChrGetByNo(dbgFldMoveIndex + 1);
        DbgPrintf(4, 5, dbgCharaMoveStrKindParty, dbgFldMoveIndex + 1, dbgFldCharaMoveNames[dbgFldMoveIndex]);
    } else {
        obj = ChrTenantAt(dbgFldMoveIndex);
        if (obj == NULL) {
            return;
        }
        DbgPrintf(4, 5, dbgCharaMoveStrKindTenant, obj->chrId);
    }
    node = ChrModelFindPart(obj->model, 0x280);
    if (node != NULL) {
        dbgFldMoveNeckX = node->rot[0] * 0.005493164f;
        dbgFldMoveNeckY = node->rot[1] * 0.005493164f;
        dbgFldMoveNeckZ = node->rot[2] * 0.005493164f;
    } else {
        dbgFldMoveNeckX = 0.0f;
        dbgFldMoveNeckY = 0.0f;
        dbgFldMoveNeckZ = 0.0f;
    }
    DbgPrintf(4, 6, dbgCharaMoveStrPosX, dbgFldMoveTargetPos[0]);
    DbgPrintf(4, 7, dbgCharaMoveStrPosY, dbgFldMoveTargetPos[1]);
    DbgPrintf(4, 8, dbgCharaMoveStrPosZ, -dbgFldMoveTargetPos[2]);
    DbgPrintf(4, 9, dbgCharaMoveStrDirect, dbgFldMoveDirect);
    DbgPrintf(4, 10, dbgCharaMoveStrSpeed, dbgFldMoveSpeed);
    DbgPrintf(4, 11, dbgCharaMoveStrNeckX, -dbgFldMoveNeckX);
    DbgPrintf(4, 12, dbgCharaMoveStrNeckY, dbgFldMoveNeckY);
    DbgPrintf(4, 13, dbgCharaMoveStrNeckZ, dbgFldMoveNeckZ);
    DbgPrintf(4, 14, dbgCharaMoveStrMove);
    DbgPrintf(4, 15, dbgCharaMoveStrSetCursor);
    DbgPrintf(4, 16, dbgCharaMoveStrCenterLine);
    copyq(&dbgFldMoveCursorPos, dbgFldMoveTargetPos);
    dbgFldDispLines |= 0x100;
    DbgDrawCursor(3, dbgFldMoveLine + 5);

    if (!(pad->held & 2)) {
        if (pad->repeatFast & 0x1000) {
            if (dbgFldMoveLine == 0) {
                dbgFldMoveLine = 11;
            } else {
                dbgFldMoveLine--;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgFldMoveLine < 11) {
                dbgFldMoveLine++;
            } else {
                dbgFldMoveLine = 0;
            }
        }
        if (pad->repeatFast & 0x8000) {
            switch (dbgFldMoveLine) {
            case 0:
                if (dbgFldMoveIndex != 0) {
                    dbgFldMoveIndex--;
                } else {
                    if (dbgFldMoveKind == 0) {
                        dbgFldMoveKind = 1;
                    } else {
                        dbgFldMoveKind = 0;
                    }
                    if (ChrTenantGetCount() == 0) {
                        dbgFldMoveKind = 0;
                    }
                    if (dbgFldMoveKind != 0) {
                        dbgFldMoveIndex = ChrTenantGetCount() - 1;
                    } else {
                        dbgFldMoveIndex = 8;
                    }
                }
                break;
            case 1:
            case 2:
            case 3:
                if (!(pad->held & 1)) {
                    dbgFldMoveTargetPos[dbgFldMoveLine - 1] -= 1.0f;
                } else {
                    dbgFldMoveTargetPos[dbgFldMoveLine - 1] -= 10.0f;
                }
                break;
            case 4:
                dbgFldMoveDirect -= 1.0f;
                if (dbgFldMoveDirect < -180.0f) {
                    dbgFldMoveDirect += 360.0f;
                }
                if (dbgFldMoveDirect >= 180.0f) {
                    dbgFldMoveDirect -= 360.0f;
                }
                break;
            case 5:
                if (dbgFldMoveSpeed != 0) {
                    dbgFldMoveSpeed--;
                }
                break;
            case 6:
                dbgFldMoveNeckX -= 1.0f;
                if (dbgFldMoveNeckX < -180.0f) {
                    dbgFldMoveNeckX += 360.0f;
                }
                if (dbgFldMoveNeckX >= 180.0f) {
                    dbgFldMoveNeckX -= 360.0f;
                }
                break;
            case 7:
                dbgFldMoveNeckY -= 1.0f;
                if (dbgFldMoveNeckY < -180.0f) {
                    dbgFldMoveNeckY += 360.0f;
                }
                if (dbgFldMoveNeckY >= 180.0f) {
                    dbgFldMoveNeckY -= 360.0f;
                }
                break;
            case 8:
                dbgFldMoveNeckZ -= 1.0f;
                if (dbgFldMoveNeckZ < -180.0f) {
                    dbgFldMoveNeckZ += 360.0f;
                }
                if (dbgFldMoveNeckZ >= 180.0f) {
                    dbgFldMoveNeckZ -= 360.0f;
                }
                break;
            }
        }
        if (pad->repeatFast & 0x2000) {
            switch (dbgFldMoveLine) {
            case 0:
                dbgFldMoveIndex++;
                if (dbgFldMoveKind == 0) {
                    if (dbgFldMoveIndex >= 9) {
                        dbgFldMoveIndex = 0;
                        dbgFldMoveKind = 1;
                        if (ChrTenantGetCount() == 0) {
                            dbgFldMoveKind = 0;
                        }
                    }
                } else {
                    if (ChrTenantGetCount() != 0) {
                        if (dbgFldMoveIndex >= ChrTenantGetCount()) {
                            dbgFldMoveKind = 0;
                            dbgFldMoveIndex = 0;
                        }
                    } else {
                        dbgFldMoveKind = 0;
                        dbgFldMoveIndex = 0;
                    }
                }
                break;
            case 1:
            case 2:
            case 3:
                if (!(pad->held & 1)) {
                    dbgFldMoveTargetPos[dbgFldMoveLine - 1] += 1.0f;
                } else {
                    dbgFldMoveTargetPos[dbgFldMoveLine - 1] += 10.0f;
                }
                break;
            case 4:
                dbgFldMoveDirect += 1.0f;
                if (dbgFldMoveDirect < -180.0f) {
                    dbgFldMoveDirect += 360.0f;
                }
                if (dbgFldMoveDirect >= 180.0f) {
                    dbgFldMoveDirect -= 360.0f;
                }
                break;
            case 5:
                dbgFldMoveSpeed++;
                break;
            case 6:
                dbgFldMoveNeckX += 1.0f;
                if (dbgFldMoveNeckX < -180.0f) {
                    dbgFldMoveNeckX += 360.0f;
                }
                if (dbgFldMoveNeckX >= 180.0f) {
                    dbgFldMoveNeckX -= 360.0f;
                }
                break;
            case 7:
                dbgFldMoveNeckY += 1.0f;
                if (dbgFldMoveNeckY < -180.0f) {
                    dbgFldMoveNeckY += 360.0f;
                }
                if (dbgFldMoveNeckY >= 180.0f) {
                    dbgFldMoveNeckY -= 360.0f;
                }
                break;
            case 8:
                dbgFldMoveNeckZ += 1.0f;
                if (dbgFldMoveNeckZ < -180.0f) {
                    dbgFldMoveNeckZ += 360.0f;
                }
                if (dbgFldMoveNeckZ >= 180.0f) {
                    dbgFldMoveNeckZ -= 360.0f;
                }
                break;
            }
        }
        if (pad->repeatFast & 0x20) {
            switch (dbgFldMoveLine) {
            case 9:
                if (obj != NULL) {
                    if (obj->actFlags & 1) {
                        obj->actFlags ^= 1;
                    }
                    if (dbgFldMoveSpeed == 0) {
                        copyq(obj->model->pos, dbgFldMoveTargetPos);
                    } else {
                        FldObjMoveTo(obj, dbgFldMoveTargetPos, 2, dbgFldMoveSpeed);
                    }
                    obj->moveDir = obj->heading = dbgFldMoveDirect * 182.04445f;
                }
                break;
            case 10:
                copyq(dbgFldMoveTargetPos, obj->model->pos);
                dbgFldMoveDirect = obj->heading * 0.005493164f;
                if (dbgFldMoveDirect < -180.0f) {
                    dbgFldMoveDirect += 360.0f;
                }
                if (dbgFldMoveDirect >= 180.0f) {
                    dbgFldMoveDirect -= 360.0f;
                }
                break;
            case 11:
                if (dbgFldMoveCenterLine != 0) {
                    dbgFldMoveCenterLine = 0;
                } else {
                    dbgFldMoveCenterLine = 1;
                }
                break;
            }
        }
    }
    dbgFldMoveCursorDir = dbgFldMoveDirect;
    if (node != NULL) {
        node->rot[0] = dbgFldMoveNeckX * 182.04445f;
        node->rot[1] = dbgFldMoveNeckY * 182.04445f;
        node->rot[2] = dbgFldMoveNeckZ * 182.04445f;
    }
    if (dbgFldMoveCenterLine != 0) {
        e[0].id = 9;
        e[0].z = 0;
        e[0].c0[0] = 0xFF;
        e[0].c0[1] = 0xFF;
        e[0].c0[2] = 0xFF;
        e[0].c0[3] = 0x80;
        e[0].x0 = 0;
        e[0].y0 = 0xE0;
        e[0].c1[0] = 0xFF;
        e[0].c1[1] = 0xFF;
        e[0].c1[2] = 0xFF;
        e[0].c1[3] = 0x80;
        e[0].x1 = 0x280;
        e[0].y1 = 0xE0;
        p = &e[1];
        p->id = 9;
        p->z = 0;
        p->c0[0] = 0xFF;
        p->c0[1] = 0xFF;
        p->c0[2] = 0xFF;
        p->c0[3] = 0x80;
        p->x0 = 0x140;
        p->y0 = 0;
        p->c1[0] = 0xFF;
        p->c1[1] = 0xFF;
        p->c1[3] = 0x80;
        p->c1[2] = 0xFF;
        p->y1 = 0x1C0;
        p->x1 = 0x140;
        e[2].id = 0;
        GphBatchDrawLines(e);
    }
}
