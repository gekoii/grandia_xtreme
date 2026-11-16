#include "common.h"
#include "pad.h"
#include "camera.h"
#include "gph.h"
#include "main/n02.h"
#include "protos/main.h"

extern s32 dbgMotPage;
extern u8 dbgMotMenuStrMotionMenuFrame[];

void DbgMotionMenu(void) {
    GphModel *ent;
    s32 n;
    f32 zero;

    if (dbgObjList == NULL) {
        return;
    }
    dbgPad = &padPort1;
    if (!(padPort1.held & 2)) {
        if (dbgPad->trigger & 4) {
            if (dbgMotPage != 0) {
                dbgMotPage--;
            } else {
                dbgMotPage = 2;
            }
        }
        if (dbgPad->trigger & 8) {
            if (dbgMotPage < 2) {
                dbgMotPage++;
            } else {
                dbgMotPage = 0;
            }
        }
    }
    if (dbgMotPage == 99) {
        DbgMotionPartsPage();
    } else {
        DbgMotionObjPage();
    }
    ent = dbgObjList->objs[dbgObjNo];
    if (ent == NULL) {
        dbgObjNo = 0;
        return;
    }
    if (!(dbgPad->held & 2)) {
        if (dbgPad->trigger & 1) {
            zero = 0.0f;
            dbgCamera->lookAt[0] = ent->pos[0];
            ent->rot[0] = 0;
            ent->rot[1] = 0;
            ent->rot[2] = 0;
            dbgCamera->lookAt[1] = ent->pos[1] + 120.0f;
            dbgCamera->lookAt[2] = ent->pos[2];
            dbgCamera->roll = zero;
            dbgCamera->yaw = zero;
        }
        if (dbgPad->leftY < 30) {
            dbgCamera->dist += (dbgPad->leftY - 30.0f) * 0.25f;
        } else if (dbgPad->leftY > 200) {
            dbgCamera->dist += (dbgPad->leftY - 200.0f) * 0.25f;
        }
        if (dbgPad->rightY < 30) {
            dbgCamera->pitch -= (f32)((30 - dbgPad->rightY) * 2) * 9.58738e-05f;
        } else if (dbgPad->rightY > 200) {
            dbgCamera->pitch += (f32)((dbgPad->rightY - 200) * 2) * 9.58738e-05f;
        }
        if (dbgPad->rightX < 30) {
            dbgCamera->roll -= (f32)((30 - dbgPad->rightX) * 2) * 9.58738e-05f;
        } else if (dbgPad->rightX > 200) {
            dbgCamera->roll += (f32)((dbgPad->rightX - 200) * 2) * 9.58738e-05f;
        }
        dbgCamera->yaw += (f32)(dbgPad->pressure[8] * 4) * 9.58738e-05f;
        dbgCamera->yaw -= (f32)(dbgPad->pressure[9] * 4) * 9.58738e-05f;
    }
    if (ent->motion[0].motion == NULL) {
        n = 0;
    } else {
        n = ent->motion[0].motion->frameCount - 1;
    }
    DbgPrintf(2, 2, dbgMotMenuStrMotionMenuFrame, ent->motion[0].frame, n);
}
