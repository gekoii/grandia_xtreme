/* Port-only C for MenuTacticsInit (0x0068E238), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "sys.h"

extern TxtGlyphBuf menuTacticsGlyphBuf;          /* 0x006E8400 */
extern u8 menuTacticsNameStrs[8][24];
extern TxtObj menuTacticsNameTexts[8];          /* 0x006E8418 */
extern TxtObj menuTacticsAiText;                /* 0x006E8538 */
extern TxtObj menuTacticsOkText;
extern u8 menuTacticsOkStr[];
extern u8 menuTacticsAiStr[];
extern u8 menuTacticsSetLetterStrs[2][3];
extern TxtObj menuTacticsSetLetterTexts[2];     /* 0x006E8560 */
extern SprColorQuad menuTacticsBgQuad;          /* 0x006E85A8 */
extern SprColorQuad menuTacticsTagQuad;         /* 0x006E85C0 */
extern SprColorQuad menuTacticsListQuad;        /* 0x006E85D8 */

/* Tactics window: member tags, lists, the tactic name texts, the colour
 * quads and the auto camera panel */
void MenuTacticsInit(void)
{
    TxtObj *s;
    s32 i;

    menuTacticsState = 0;
    TxtGlyphBufInit(&menuTacticsGlyphBuf, sysFontImage2, 100, 0xE3);
    for (i = 0; i < 4; i++) {
        menuTacticsTags[i].y = 58.0f;
        menuTacticsTags[i].slide = 0;
        menuTacticsTags[i].alpha = 0.0f;
        menuTacticsTags[i].fade = (btlTacticsLabelsOn != 0) ? (btlUnits[i].ai != 0) : 0;
    }
    for (i = 0; i < 4; i++) {
        menuTacticsLists[i].state = -1;
    }
    menuTacticsBannerTimer = 0;

    for (i = 0; i < 8; i++) {
        s = &menuTacticsNameTexts[i];
        s->alignX = 0.5f;
        s->alignY = 0.0f;
        s->x = 3;
        s->y = 4;
        s->scale = 1.0f;
        s->style = TXT_STYLE_OUTLINE;
        s->spacing = 2;
        TxtObjSetString(s, &menuTacticsGlyphBuf, menuTacticsNameStrs[i]);
    }
    s = &menuTacticsAiText;
    s->spacing = 2;
    s->style = TXT_STYLE_OUTLINE;
    s->scale = 1.0f;
    TxtObjSetString(&menuTacticsOkText, &menuTacticsGlyphBuf, menuTacticsOkStr);
    TxtObjSetString(s, &menuTacticsGlyphBuf, menuTacticsAiStr);
    for (i = 0; i < 2; i++) {
        s = &menuTacticsSetLetterTexts[i];
        s->style = TXT_STYLE_OUTLINE;
        s->scale = 1.0f;
        s->spacing = 2;
        TxtObjSetString(s, &menuTacticsGlyphBuf, menuTacticsSetLetterStrs[i]);
    }

    menuTacticsTagQuad.w = 0x6E;
    menuTacticsTagQuad.originX = 0x37;
    menuTacticsTagQuad.originY = 0;
    menuTacticsTagQuad.col[0].w = 0x4040F010;
    menuTacticsTagQuad.col[1].w = 0x7060;
    menuTacticsTagQuad.col[2].w = 0x4040F010;
    menuTacticsTagQuad.col[3].w = 0x7060;

    menuTacticsBgQuad.w = 0x280;
    menuTacticsBgQuad.h = 0x50;
    menuTacticsBgQuad.originX = 0;
    menuTacticsBgQuad.originY = 0x28;
    menuTacticsBgQuad.col[0].w = 0x4040F050;
    menuTacticsBgQuad.col[1].w = 0x6078;
    menuTacticsBgQuad.col[2].w = 0x4040F050;
    menuTacticsBgQuad.col[3].w = 0x6078;

    menuTacticsListQuad.w = 0x6E;
    menuTacticsListQuad.originX = 0x37;
    menuTacticsListQuad.originY = -0x1A;
    menuTacticsListQuad.col[0].w = 0x7060;
    menuTacticsListQuad.col[1].w = 0x6078;
    menuTacticsListQuad.col[2].w = 0x7060;
    menuTacticsListQuad.col[3].w = 0x6078;

    MenuLrArrowsSet(320.0f, 256.0f, 280.0f);

    menuAutoCamPanel.state = 0;
    menuAutoCamPanel.mode = 0;
    menuAutoCamPanel.alpha = 0.0f;
    menuAutoCamPanel.slide = 64.0f;
    menuAutoCamPanel.slideVel = 0.0f;
    menuAutoCamPanel.cursorX = 0.0f;
    menuAutoCamPanel.cursorVel = 0.0f;
    menuAutoCamPanel.quad.w = 0x20;
    menuAutoCamPanel.quad.h = 0x20;
    menuAutoCamPanel.quad.originX = 0;
    menuAutoCamPanel.quad.originY = 0;
    menuAutoCamPanel.quad.col[0].w = 0x2EAC8759;
    menuAutoCamPanel.quad.col[1].w = 0x03567159;
    menuAutoCamPanel.quad.col[2].w = 0x2EAC8759;
    menuAutoCamPanel.quad.col[3].w = 0x03567159;
}
