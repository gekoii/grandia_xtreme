/*
 * Names whose address is computed from a label rather than being inside an
 * object, so tools/c_syms.py can't place them. Every binary is linked with
 * this file; a PROVIDE only takes effect in a binary that uses the name.
 */

/* BATTLE: the panel rect table (12-byte rects at ipPanelFrameRect + 0xC) is
   indexed by character id, which starts at 0x1001, so the code's base is
   0x1001 entries below the table (0x006D3320, inside .text). */
PROVIDE(ipPanelCharRects = ipPanelFrameRect + 0xC - 0x1001 * 12);

/* BATTLE: the target area names (13-byte font-coded strings, one per target
   type 1-7, src/battle/data/6E0198.c) are indexed by type from a base one
   entry below the table: the name of type 0, which doesn't exist. */
PROVIDE(btlTargetAreaNames = btlTargetAreaNameText - 13);

/* main: the item help texts (src/main/data/4A2A90.c) index three tables
   from biased bases: element marks by element 1-8, element names by k * 9
   for k = 5..8, status names by status bit 4-16 (17 bytes each). */
PROVIDE(menuHelpElementLabels = menuHelpElementMarks - 1);
PROVIDE(menuHelpElementNames = menuHelpElementTexts - 0x2D);
PROVIDE(menuHelpStatusNames = menuHelpStatusTexts - 0x44);

/* FIELD: TxtWinTextureBind indexes the window textures from 3 (0x680 bytes
   each, slot 3 = txtMsgWinTexture), so the code's base is 3 entries below:
   inside camDebugTriBuf. */
PROVIDE(txtWinTexturesBase = txtMsgWinTexture - 3 * 0x680);
