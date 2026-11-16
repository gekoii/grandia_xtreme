/* Port-only C for FldMenuWinTextAdd (0x00621400), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "field/menu.h"

extern const u8 fldMenuTextOutlineColors[6][4];
extern const u8 fldMenuTextDefaultColor[4];

void FldMenuWinGlyphsBuild(void *q, s32 *str, s32 x, s32 y, s32 size, s32 h, s32 vbase, s32 ubase, s32 n,
                           u8 *col2, u8 *col1, u8 mode);

/* codes: the game's frame has 32 (0x80 bytes at sp+0x20). The field menu's
   location (FldMenuTopInit, up to 0x80 bytes) can be longer: "Aqua Ruins
   - Energy Conversion Room" (map 0x50F0) is 35. On the PS2 its codes past 32 run over
   the colour tables and spare space below the saved registers; here they
   reached the saved registers, and the caller crashed when it returned (the
   field menu, Triangle). A message is at most 255 codes (its length is a
   byte), a string as long as its buffer. */
s32 FldMenuWinTextAdd(u8 id, u8 slot, u8 *msg, s32 x, s32 y, s32 size, s32 h, s32 pos, s32 idx, s32 mode) {
    s32 codes[256];
    u8 cols[6][4];
    u8 white[4];
    FldMenuWin *w;
    FldMenuQuad *q;
    s32 n;
    s32 ubase;
    s32 i;

    /* The game copies both colour tables to its stack and passes those */
    for (i = 0; i < 6; i++) {
        cols[i][0] = fldMenuTextOutlineColors[i][0];
        cols[i][1] = fldMenuTextOutlineColors[i][1];
        cols[i][2] = fldMenuTextOutlineColors[i][2];
        cols[i][3] = fldMenuTextOutlineColors[i][3];
    }
    for (i = 0; i < 4; i++) {
        white[i] = fldMenuTextDefaultColor[i];
    }
    w = &fldMenuWins[id];

    if (idx != -1) {
        n = TxtGlyphQueueMsg(slot, (struct TxtMsgHdr *)msg, pos, idx);
    } else {
        n = TxtGlyphQueueStr(slot, msg, pos);
    }
    w->txtWinId = slot;
    q = &w->quads[82 + w->glyphCount];
    if (pos >= 14) {
        ubase = 0xFC;
        pos -= 14;
    } else {
        ubase = 0;
    }
    TxtMsgGetCodes(msg, idx, codes);

    /* The window's glyphs go in its quads from 82 to 883, 2 or 5 a
       character by the mode; the game doesn't check. The port's Controls
       list, with long key names, could run past them into the window's
       layer pointers: the characters that don't fit are left out */
    {
        s32 per = (mode >= 1 && mode <= 6) || mode == 8 || mode == 48 ? 5
                  : mode == 0 || mode == 10 || mode == 40           ? 2 : 0;
        s32 room = (s32)(sizeof(w->quads) / sizeof(w->quads[0])) - 82 - (s32)w->glyphCount;
        if (per != 0 && n * per > room) {
            n = room > 0 ? room / per : 0;
        }
    }

    switch (mode) {
    case 10:
        FldMenuWinGlyphsBuild(q, codes, x, y, size, h, pos, ubase, n, cols[5], cols[0], 0);
        w->glyphCount += n * 2;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        FldMenuWinGlyphsBuild(q, codes, x, y, size, h, pos, ubase, n, white, cols[mode > 3 ? mode - 3 : mode], 1);
        w->glyphCount += n * 5;
        break;
    case 0:
    case 40:
        FldMenuWinGlyphsBuild(q, codes, x, y, size, h, pos, ubase, n, white, cols[0], 0);
        w->glyphCount += n * 2;
        break;
    case 8:
    case 48:
        FldMenuWinGlyphsBuild(q, codes, x, y, size, h, pos, ubase, n, white, cols[4], 1);
        w->glyphCount += n * 5;
        break;
    }
    return n;
}
