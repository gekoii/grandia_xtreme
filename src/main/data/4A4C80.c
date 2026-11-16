/* main executable .rodata 0x004A4C80-0x004A4D10: strings of
 * the debug menu's wipe and fade pages.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A4C80-0x004A4D10 */

/* DbgWipePage (428118.c) */
const char dbgWipeStrWipe[] = "WIPE";
const char dbgWipeStrType[] = "TYPE    : %d";
const char dbgWipeStrParam0[] = "PARAM 0 : %d";
const char dbgWipeStrParam1[] = "PARAM 1 : %d";

/* DbgFadePage (428370.c) */
const char dbgFadeStrFade[] = "FADE";
const char dbgFadeStrR[] = "R    : %.3f";
const char dbgFadeStrG[] = "G    : %.3f";
const char dbgFadeStrB[] = "B    : %.3f";
const char dbgFadeStrA[] = "A    : %.3f";
const char dbgFadeStrTime[] = "TIME : %.3f";
