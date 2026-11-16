/* main executable: the .rodata of a file that has nothing else in it (the
 * map set file functions, MapSetFileReset 41F950 and its neighbours, have
 * no constants): only the header's "save". Data-only C (plan step 6b,
 * docs/shifting.md).
 */
/* rodata: 0x004A35E0-0x004A35E8 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";
