#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/menu.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);
void FldMenuPartyPanelOpen(s32, s32, s32);
s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
FldMenuQuad *FldMenuWinGetSprBuf(u8, s32);

void FldMenuTopInit(void) {
    char buf[0x80];
    FldMenuQuad *s;

    MemSet(&fldMenuPartyPanel, 0, sizeof(FldMenuPartyPanel));
    MemSet(buf, 0, 0x80);
    FldMenuPartyPanelOpen(2, 0x140, 0xE0);
    FldMenuWinOpen(0, 0x140, 0x1A8, 0x200, 0x1C, 0, 8, 1);
    FldMenuWinTextAdd(0, 2, sysGrxsysPac->msg->tbl[0], 4, 3, 0xE, 0x14, 0x14, 0x26, 2);
    EvtMsgGetText(0xE000, buf, 0);
    FldMenuWinTextAdd(0, 2, (s32)buf, 0x50, 3, 0x14, 0x14, 0xC, -1, 8);
    FldMenuWinOpen(1, 0x1C0, 0x17A, 0xC8, 0x1C, 3, 8, 1);
    FldMenuWinTextAdd(1, 2, sysGrxsysPac->msg->tbl[0], 4, 3, 0x14, 0x14, 0x15, 0x25, 1);
    FldMenuWinTextAdd(1, 2, sysGrxsysPac->msg->tbl[0], 0xB0, 6, 0x10, 0x10, 0x17, 0x34, 2);
    s = FldMenuWinGetSprBuf(1, 1);
    FldMenuWinSetLayer(1, 1, fldMenuTextures, s, 128.0f);
    ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s, itemWork->gold, 1, 0xB0, 4, 0x80, 1.0f, 0.9f);
}
