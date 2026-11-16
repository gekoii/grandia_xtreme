/* Port-only C for FldMenuSaveSlotListBuild (0x006354E0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "mc.h"
#include "field/n23.h"
#include "field/n24.h"
#include "field/n26.h"

extern GphBatchQuad *fldMenuSaveSprMisc;
extern GphBatchQuad *fldMenuSaveSprFaces;
extern GphBatchQuad *fldMenuSaveSprLabels;
extern GphBatchQuad *fldMenuSaveSprNumbers;
extern GphBatchQuad *fldMenuSaveQuads;
extern s8 fldMenuSaveCardHasSpace;

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* The four sprite lists and the quad buffer of a save row window (8: the
   slot list, 0xA: the game being saved or loaded), from sprite buffer
   first..first+3. The asm addresses the textures from fldMenuTex3 (+0x3C0,
   -0x140, -0x3C0: fldMenuTextures[6], [2], [0]); the port names them, since
   its BSS objects needn't be laid out like the game's. */
static void FldMenuSaveRowLayersSet(u8 win, u8 first) {
    fldMenuSaveSprMisc = FldMenuGetSprBuf(first);
    FldMenuWinSetLayer(win, 0, &fldMenuTex3, fldMenuSaveSprMisc, 128.0f);
    fldMenuSaveSprFaces = FldMenuGetSprBuf(first + 1);
    FldMenuWinSetLayer(win, 1, &fldMenuTexFaces, fldMenuSaveSprFaces, 128.0f);
    fldMenuSaveSprLabels = FldMenuGetSprBuf(first + 2);
    FldMenuWinSetLayer(win, 2, &fldMenuTexLabels2, fldMenuSaveSprLabels, 128.0f);
    fldMenuSaveSprNumbers = FldMenuGetSprBuf(first + 3);
    FldMenuWinSetLayer(win, 3, &fldMenuTextures[0], fldMenuSaveSprNumbers, 128.0f);
    fldMenuSaveQuads = (GphBatchQuad *)FldMenuWinGetQuadBuf(win);
}

/* End markers after the rows */
static void FldMenuSaveRowLayersEnd(void) {
    fldMenuSaveSprMisc->type = 0;
    fldMenuSaveSprFaces->type = 0;
    fldMenuSaveSprLabels->type = 0;
    fldMenuSaveSprNumbers->type = 0;
    fldMenuSaveQuads->type = 0;
}

/* mode 0 opens the windows, 2 closes them after building; sel is the slot
   cursor (0-7), flags bit 1 "the card has space". */
void FldMenuSaveSlotListBuild(s32 mode, s32 sel, s32 flags) {
    McSlotTable *tbl;
    McSlot *slot;
    s32 font;
    s32 top;
    s32 row;
    s32 i;
    s32 y;
    s32 n;

    fldMenuSaveCardHasSpace = (flags >> 1) & 1;
    if (mode == 0) {
        FldMenuWinOpen(9, 0x1B8, 0x30, 0x140, 0x1E, 0, 2, 0x8001);
        FldMenuWinOpen(8, 0x140, 0xC8, 0x230, 0x100, 0, 0, 0x9001);
        FldMenuWinOpen(0xA, 0x140, 0x186, 0x230, 0x58, 0, 3, 0x8001);
    }
    FldMenuWinTextClear(9);
    FldMenuWinTextClear(0xA);
    FldMenuWinTextClear(8);
    tbl = McGetSlotTable();
    font = sysGrxsysPac->msg->tbl[11];
    FldMenuWinTextAdd(9, 1, font, 0x20, 4, 0x14, 0x14, 3, 3, 0);
    FldMenuWinTextAdd(9, 1, font, 0xD0, 4, 0x14, 0x14, 4, fldMenuSaveCardSlot + 4, 0);
    FldMenuWinTextAdd(8, 1, font, 0x28, -2, 0x18, 0x14, 5, fldMenuSaveIsLoad, 1);
    FldMenuSaveRowLayersSet(8, 4);

    /* Three rows of the eight slots, the cursor kept on the middle one
       where it can be */
    top = 0;
    if (sel > 1 && sel < 7) {
        top = sel - 1;
    }
    if (sel >= 7) {
        top = 5;
    }
    row = sel;
    if (sel > 0 && sel < 7) {
        row = 1;
    }
    if (sel >= 7) {
        row = sel - 5;
    }
    n = 6;
    for (i = 0, y = 0x1E; i < 3; i++, y += 0x4A) {
        slot = &tbl->slots[top + i];
        if (sel == top + i) {
            FldMenuSaveFileRowBuild(8, 0x18, y, slot, sel, n, 1);
        } else {
            FldMenuSaveFileRowBuild(8, 0x28, y, slot, top + i, n, 0);
        }
        n += 3;
    }
    FldMenuSaveRowLayersEnd();

    FldMenuSaveRowLayersSet(0xA, 8);
    if (fldMenuSaveIsLoad == 0) {
        sel = -1;
        slot = &mcSaveSlotHeader;
    } else {
        slot = &tbl->slots[sel];
    }
    FldMenuSaveFileRowBuild(0xA, 0x20, 6, slot, sel, n, 0);
    FldMenuSaveRowLayersEnd();

    FldMenuCursorSetAt(8, 16.0f, (f32)(row * 0x4A) + 40.0f);
    if (mode == 2) {
        FldMenuWinClose(9, 0xA, 2);
        FldMenuWinClose(8, 0xA, 0);
        FldMenuWinClose(0xA, 0xA, 3);
    }
}
