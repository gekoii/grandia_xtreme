#include "common.h"
#include "protos/battle.h"
#include "camera.h"
#include "battle/n29.h"
#include "sys.h"

extern char btlCmdNames[8][0x1B];
extern char btlCmdStrDefense[];

void BtlCmdMenuInit(void) {
    s32 i;
    TxtObj *t;

    btlCmdMenu.state = 0;
    TxtGlyphBufInit(&btlCmdMenuGlyphBuf, sysFontImage2, 0xE4, 0x11B);
    t = btlCmdMenuNameTexts;
    for (i = 0; i < 8; i++) {
        t->alignX = 0.0f;
        t->alignY = 1.0f;
        t->x = 4;
        t->y = 0;
        t->scale = 1.0f;
        t->style = 0x22;
        t->spacing = 2;
        TxtObjSetString(t, &btlCmdMenuGlyphBuf, btlCmdNames[i]);
        t++;
    }
    t = &btlCmdMenuDefenseText;
    t->alignX = 0.5f;
    t->alignY = 1.0f;
    t->x = 3;
    t->y = -16;
    t->scale = 0.8333333f;
    t->style = 2;
    t->spacing = 2;
    TxtObjSetString(t, &btlCmdMenuGlyphBuf, btlCmdStrDefense);
    CamInit(&btlCmdMenuCam, 0);
    btlCmdMenuCam.mode = CAM_MODE_NORMAL;
    btlCmdMenuCam.fov = 60.0f;
    btlCmdMenuCam.dist = 800.0f;
    btlCmdMenuCam.lookAt[1] = 100.0f;
    btlCmdMenuCam.lookAt[0] = 0.0f;
    btlCmdMenuCam.lookAt[2] = 0.0f;
}
