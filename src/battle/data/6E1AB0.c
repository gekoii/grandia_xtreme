/* BATTLE .rodata 0x006E1AB0-0x006E1AF0: the end of the .rodata of the
 * original file of 6BC620.c (the character file names of BtlCharFileName,
 * 6BCCA0.c), then the start of the next one (6BCCD0.c's). Data-only C (plan
 * step 6b, docs/shifting.md).
 */
/* rodata: 0x006E1AB0-0x006E1AF0 */

/* BtlCharFileName: party and enemy character files */
const char btlStrPartyFile[] = "BTL_DATA/P%.3x%s";
const char btlStrEnemyFile[] = "BTL_DATA/E%.3X%s";
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
