/* cflags: -fno-cse-follow-jumps */
/* Without -fno-cse-follow-jumps, 27 words differ: temporary registers (a1/a2/v1) swap in the panel and list code. */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "sys.h"

typedef struct {
    s16 w;
    s16 h;
} Rect;

typedef struct {
    u8 pad[0x24];
} Ent;

typedef struct {
    u8 pad[8];
    Ent *p8;
} Spr;

extern u8 menuTacticsGlyphBuf[];
extern Rect menuTacticsEditSprite;
extern u8 menuTacticsBgQuad[];
extern Rect menuTacticsListQuad;
extern Ent menuTacticsNameTexts[];
extern Ent menuTacticsSetTexts[];
extern Ent menuTacticsOkText;
extern Spr menuTacticsAiLabel;

f32 VuSinAngle(s32);

void MenuTacticsDraw(void) {
    f32 t;
    f32 sc;
    f32 sc0;
    s32 k;
    f32 y;
    s32 i;
    s32 n;
    s32 m;
    s32 j;
    s32 a;
    MenuTacticsList *p;
    MenuTacticsTag *q;
    BtlUnit *u;

    if (btlPhase == 0x80 || btlPhase == 1) {
        if ((menuTacticsState > 0 || btlTacticsLabelsOn != 0 || menuTacticsBannerTimer > 0) && btlEnemyAliveCount > 0) {
            if (menuTacticsState > 0) {
                SprBegin(0.0f, 256.0f);
                t = (f32)(8 - menuTacticsOpenTimer) * 0.125f;
                sprState.sy = VuSinAngle(t * 16384.0f);
                if (menuTacticsState == 1) {
                    SprDrawColorQuadTinted(menuTacticsBgQuad, 0xA0A0FF00, t,
                                  (VuSinAngle(btlTickCount * 0x300) + 1.0f) * 0.5f * 0.3f);
                } else {
                    SprDrawColorQuad(menuTacticsBgQuad, t * 0.5f);
                }
                SprFlush();
                GphTexBind(0xEC);
                if (menuTacticsState == 1 && menuTacticsOpenTimer <= 0) {
                    SprBegin(320.0f, 204.0f);
                    MenuButtonPadDraw(0x1000, -50, -36);
                    SprDrawRect(&menuTacticsEditSprite, -menuTacticsEditSprite.w / 2, -menuTacticsEditSprite.h / 2);
                    SprFlush();
                }
                MenuLrArrowsDraw();
            }
            TxtGlyphBufUpload(menuTacticsGlyphBuf);
            if (menuTacticsBannerTimer > 0) {
                SprBegin(320.0f, 116.0f);
                if (menuTacticsBannerTimer < 16) {
                    sprState.col[0].a = menuTacticsBannerTimer * 8.0f;
                }
                sprState.sx = 1.3333334f;
                sprState.sy = 1.3333334f;
                MenuTextSetAddColor(0x50, 0, 0);
                if (btlAutoTactics == 0) {
                    sprState.oy -= 32.0f;
                    MenuTextDraw(menuTacticsNameTexts);
                } else {
                    menuTacticsAiLabel.p8 = &menuTacticsSetTexts[btlAutoTactics];
                    MenuTextListMeasure(&menuTacticsAiLabel);
                    MenuTextListDraw(&menuTacticsAiLabel);
                }
                SprFlush();
            }
            if (menuTacticsState > 0) {
                SprBegin(200.0f, 256.0f);
                sprState.sx = 1.3333334f;
                sprState.sy *= 1.3333334f;
                sprState.col[0].a = (f32)(8 - menuTacticsOpenTimer) * 0.125f * 128.0f;
                for (n = 1; n < 3; n++) {
                    if (n != menuTacticsSet) {
                        SprSetColor(0x30, 0x30, 0x30);
                        MenuTextSetAddColor(0x20, 0, 0);
                    } else {
                        MenuTextSetAddColor(0x50, 0, 0);
                        SprSetColor(0x80, 0x80, 0x80);
                        if (menuTacticsState == 1) {
                            MenuTextDraw(&menuTacticsOkText);
                        }
                        MenuTextSetAddColor(0x50, 0, 0);
                    }
                    menuTacticsAiLabel.p8 = &menuTacticsSetTexts[n];
                    MenuTextListMeasure(&menuTacticsAiLabel);
                    MenuTextListDraw(&menuTacticsAiLabel);
                    sprState.ox += 240.0f;
                }
                SprFlush();
                p = menuTacticsLists;
                for (i = 0; i < 4; i++, p++) {
                    if (p->state >= 0) {
                    SprBegin(i * 160.0f + 80.0f, menuTacticsTags[i].y);
                    menuTacticsListQuad.h = p->alpha * 130.0f;
                    if (menuTacticsState == 2 && i == menuTacticsMember) {
                        SprDrawColorQuadTinted(&menuTacticsListQuad, 0xA0A0FF00, menuTacticsTags[i].alpha,
                                      (VuSinAngle(btlTickCount * 0x300) * 0.5f + 0.5f) * 0.3f);
                    } else {
                        SprDrawColorQuad(&menuTacticsListQuad, menuTacticsTags[i].alpha * 0.5f);
                    }
                    SprFlush();
                    sprState.sy = 0.6666667f;
                    sprState.sx = 0.6666667f;
                    sc0 = p->alpha;
                    sprState.col[0].a = sc0 * 128.0f;
                    if (p->sel != 0) {
                        sprState.oy += sc0 * 4.0f;
                    }
                    for (j = 0; j < 8; j++) {
                        MenuTextSetAddColor(0x30, 0x10, 0);
                        if (j == p->sel) {
                            sc = p->alpha;
                            sprState.oy += sc * 22.0f;
                        } else {
                            if (j == btlUnits[i].ai) {
                                SprSetColor(0x80, 0x70, 0);
                                MenuTextDraw(&menuTacticsNameTexts[j]);
                                SprSetColor(0x80, 0x80, 0x80);
                            } else {
                                MenuTextDraw(&menuTacticsNameTexts[j]);
                            }
                            sc = p->alpha;
                            sprState.oy += sc * 18.0f;
                        }
                    }
                    sprState.col[0].a = 0x80;
                    k = p->sel;
                    y = menuTacticsTags[i].y + k * 18.0f * sc;
                    sprState.oy = y;
                    if (k != 0) {
                        sprState.oy = y + sc * 4.0f;
                    }
                    sprState.sy = 0.8333333f;
                    sprState.sx = 0.8333333f;
                    MenuTextSetAddColor(0x70, 0x10, 0x10);
                    if (p->sel == btlUnits[i].ai) {
                        SprSetColor(0x80, 0x70, 0);
                        MenuTextDraw(&menuTacticsNameTexts[p->sel]);
                        SprSetColor(0x80, 0x80, 0x80);
                    } else {
                        MenuTextDraw(&menuTacticsNameTexts[p->sel]);
                    }
                    SprFlush();
                    }
                }
            }
            if (btlTacticsLabelsOn != 0 && !(btlHudHideFlags & 2)) {
                if (btlAutoTactics == 0 || menuTacticsBannerTimer > 0 || !(btlTickCount & 0x20)) {
                    a = 0x80;
                } else {
                    a = 0;
                }
                SprBegin(80.0f, 58.0f);
                sprState.sy = 0.8333333f;
                sprState.sx = 0.8333333f;
                if (btlAutoTactics != 0 && (btlTickCount & 8)) {
                    SprSetColor(0x80, 0x80, 0x50);
                } else {
                    SprSetColor(0x80, 0x70, 0);
                }
                q = menuTacticsTags;
                u = btlUnits;
                for (m = 0; m < 4; m++, q++, u++) {
                    if (u->state >= 2 && !(u->flags & 0x40) && menuTacticsLists[m].state < 0) {
                        sprState.oy = q->y;
                        sprState.col[0].a = a * q->alpha;
                        MenuTextSetAddColor(0x30, 0x10, 0);
                        MenuTextDraw(&menuTacticsNameTexts[u->ai]);
                    }
                    sprState.ox += 160.0f;
                }
                SprFlush();
            }
            if (!(btlHudHideFlags & 8) && menuAutoCamPanel.state != 0) {
                ((s32 (*)(void))MenuAutoCamPanelDraw)(); /* cast: no tail call */
            }
        }
    }
}
