#include "common.h"
#include "protos/battle.h"
#include "camera.h"
#include "gph.h"
#include "battle/n29.h"
#include "sys.h"

f32 VuSinAngle(s32);
void VuMtxStackPush(u32);
void VuMtxStackLoad(void *);
extern u16 btlCmdMenuHues[];
extern const SprRect btlCmdMenuActorMarker;
extern const SprRect btlCmdIconSprites[];

void BtlCmdMenuDraw(void) {
    BtlCmdMenu *m = &btlCmdMenu;
    VuVec4 v;
    u32 c;
    u16 hue;

    if (m->state == 0) {
        return;
    }
    if (btlTickCount & 0x10) {
        GphTexBind(0xED);
        SprBegin(m->unit->id * 160.0f + 8.0f, 16.0f);
        SprDrawRect(&btlCmdMenuActorMarker, 0, 0);
        SprFlush();
    }
    if (m->state == 0x41 && m->unkAC == 0) {
        UtilWorldToScreen(&v, (VuVec4 *)btlCmdMenuAllyCursor->pos);
        SprBegin(v.x, v.y - 16.0f);
        GphTexBind(0xF1);
        sprState.sy = 1.5f;
        sprState.sx = 1.5f;
        SprDrawRect(&btlCmdIconSprites[1], -btlCmdIconSprites[1].w / 2, -btlCmdIconSprites[1].h / 2);
        sprState.sy = 1.0f;
        sprState.sx = 1.0f;
        SprFlush();
        TxtGlyphBufUpload(&btlCmdMenuGlyphBuf);
        hue = btlCmdMenuHues[1];
        c = UtilHsvToRgb(hue, 0.95f, VuSinAngle(btlTickCount << 10) * 0.1f + 0.2f);
        MenuTextSetAddColor(c >> 24, c >> 16, c >> 8);
        MenuTextDraw(&btlCmdMenuDefenseText);
        SprFlush();
    }
    if (m->state == 0x10) {
        VuMtxStackPush(0);
        CamApply(&btlCmdMenuCam);
        VuMtxStackLoad(&btlCmdMenuCam);
        GphDrawListSet(btlCmdMenuRingDrawList);
        GphDrawListDrawAll();
        VuMtxStackPop();
        CamApply(btlCamera);
        VuMtxStackLoad(btlCamera);
    }
    if (m->unkB0 > 0) {
        SprBegin(m->unkB8, m->unkBC);
        if (m->unkB0 >= 16) {
            GphTexBind(0xF1);
            SprDrawRect(&btlCmdIconSprites[m->sel], -24, -28);
            SprFlush();
        }
        TxtGlyphBufUpload(&btlCmdMenuGlyphBuf);
        hue = btlCmdMenuHues[m->sel];
        c = UtilHsvToRgb(hue, 0.95f, VuSinAngle(btlTickCount << 10) * 0.1f + 0.2f);
        MenuTextSetAddColor(c >> 24, c >> 16, c >> 8);
        MenuTextDraw(&btlCmdMenuNameTexts[m->sel]);
        SprFlush();
    }
}
