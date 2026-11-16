/* main executable: the debug FRAMEBUFFER menu's title (DbgFbMenu
 * 424D80) and the PAD page's (DbgPadPage 424E90, whose other texts are in
 * 4A4230.c). Part of the .rodata of the file that 4A36A0.c starts.
 * Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x004A4158-0x004A41A0 */

/* DbgFbMenu: page %d of 3 between dbgStrPageHiliteOn/Off */
const char dbgFbMenuStrFramebufferMenu3[] = "FRAMEBUFFER MENU %s %d/3%s ";
/* DbgPrintf colour escapes around the page number of the paged menus' titles (also
 * DbgFileMenu 4256F8, DbgSoundMenu 426620, DbgObjAtrMenu 4293A0) */
const char dbgStrPageHiliteOn[] = "\\c5 ";
const char dbgStrPageHiliteOff[] = "\\c0 ";
/* DbgPadPage */
const char dbgPadStrPadInformationMenu[] = "PAD INFORMATION MENU";
