/* BATTLE: the start of the .rodata of the file with BtlStartUpdate
 * (6A5B00.c): the encounter captions. Data-only C (plan step 6b,
 * docs/shifting.md).
 */
/* rodata: 0x006E0E38-0x006E0E80 */

typedef unsigned int u32;

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E0E38[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Font-coded captions (each character a byte, then 0xF9), by encounter
 * type */
const char btlStrForestall[] = "e\371\216\371\221\371\204\371\222\371\223\371\200\371\213\371\213\371"; /* "Forestall" (type 1) */
const char btlStrSurprised[] = "r\371\224\371\221\371\217\371\221\371\210\371\222\371\204\371\203\371@\371"; /* "Surprised!" (type 2) */
