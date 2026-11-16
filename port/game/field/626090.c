/* Port-only C for FldMenuPartyPanelOpen (0x00626090), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/menu.h"
#include "field/n23.h"

extern const u16 fldMenuPartyPanelW;
extern const u16 fldMenuPartyPanelH;

s32 FldMenuWinTextAdd(u8 id, u8 slot, u8 *msg, s32 x, s32 y, s32 size, s32 h, s32 pos, s32 idx, s32 mode);

/* Opens one panel window per party member in a 2x2 grid (base + n), with
   the portrait and the name. A panel position the switch doesn't set keeps
   the previous one, as in the game. */
void FldMenuPartyPanelOpen(u8 base, s16 ax, s16 ay) {
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    s32 i;
    s32 n;
    s32 tex;
    s32 chr;
    s32 w;
    u8 id;

    x = (s16)(ax - 0x8F);
    y = (s16)(ay - 0x30);
    px = 0;
    py = 0;
    n = 0;
    tex = 0;
    MemSet(&fldMenuPartyPanel, 0, 0x2C);
    for (i = 0; i < 4; i++) {
        chr = itemWork->chars[i].chr;
        if (chr == 0) {
            continue;
        }
        chr--;
        switch (n) {
        case 0:
            px = x;
            py = y;
            break;
        case 1:
            px = x + 0x10E + 0x10;
            py = y;
            break;
        case 2:
            px = x;
            py = y + 0x64 + 0x10;
            break;
        case 3:
            px = x + 0x10E + 0x10;
            py = y + 0x64 + 0x10;
            break;
        }
        id = base + n;
        FldMenuWinOpen(id, px, py, fldMenuPartyPanelW, fldMenuPartyPanelH, tex, 8, 0x90A0);
        tex += 3;
        fldMenuPartyPanel.hp[i] = itemWork->chars[i].hp;
        fldMenuPartyPanel.sp[i] = itemWork->chars[i].sp;
        n++;
        FldMenuWinSetPortrait(id, i, ((chr + 1) * 100) & 0xFFFC, 0, 4);
        w = TxtMsgGetWidth((struct TxtMsgHdr *)sysGrxsysPac->msg->tbl[2], chr, 0x18);
        FldMenuWinTextAdd(id, 2, (u8 *)sysGrxsysPac->msg->tbl[2], 0x54 - w / 2, -4, 0x18, 0x14, i + 0x10, chr, 1);
    }
    FldMenuPartyPanelUpdate(base);
}
