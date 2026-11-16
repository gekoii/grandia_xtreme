/* FIELD .rodata 0x006738F0-0x00673920: data-only C (plan step 6b, docs/shifting.md).
 * The start of DbgParamUpdate's file (5ED820): its "save" and the title
 * format of the PARAM debug pages. Split from the planned 6738E8.c at the
 * save.
 * Check with tools/check_data.py. Do not include the project headers here.
 */
/* rodata: 0x006738F0-0x00673920 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
const char dbgParamTitleFmt[] = "PARAM DEBUG MODE %d/%d:%s"; /* 006738F8; DbgParamUpdate (5ED820) */
