/* main executable .rodata 0x004A5488-0x004A54C0: strings of
 * the debug menu's parts attribute page.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A5488-0x004A54C0 */

/* DbgPartsAtrPage (42A320.c) */
const char dbgPartsAtrStrPartsAtrMenu[] = "PARTS ATR MENU";
const char dbgPartsAtrStrPartsNo[] = "PARTS NO    %d";
const char dbgPartsAtrStrFogOn[] = "FOG          ON";
