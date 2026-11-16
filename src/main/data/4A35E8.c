/* main executable: the start of a file's .rodata - the debug battle
 * setup's file name parts (DbgChrPathMake 41F998, DbgEnemyGbxPathMake
 * 41FA78, DbgPartyGbxPathMake 41FB78). The file's .rodata goes on after
 * 41FC18.c's constants in 4A3640.c. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x004A35E8-0x004A3630 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* "DEBUG/" + 4 hex digits + ".CHR" (DbgChrPathMake) */
const char dbgPathChrDir[] = "DEBUG/";
const char dbgPathChrExt[] = ".CHR";
/* "DEBUG/E/" + id + "_" + slot + ".GBX" (DbgEnemyGbxPathMake) and
 * "DEBUG/P/P00" + n + "_" + slot + ".GBX" (DbgPartyGbxPathMake) */
const char dbgPathEnemyDir[] = "DEBUG/E/";
const char dbgPathSep[] = "_";
const char dbgPathGbxExt[] = ".GBX";
const char dbgPathPartyPrefix[] = "DEBUG/P/P00";
