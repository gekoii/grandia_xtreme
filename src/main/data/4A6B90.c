/* main executable .rodata 0x004A6B90-0x004A6C48: the movie module's
 * strings. Data-only C (plan step 6b, docs/shifting.md).
 *
 * The 7 file names are the string literals of movTrpFileTable (.data
 * 0x0049DC58, "MOV/TRP0.BIN".."MOV/TRP6.BIN"), in the order gcc 2.9 emits a
 * table's literals (last element first). That table is in another data file
 * (6b-03's 49D6A8.c), so here they are named globals; one file holding both
 * ranges could write the table with literals instead.
 */
/* rodata: 0x004A6B90-0x004A6C48 */

typedef unsigned char u8;

/* movTrpFileTable[6]..[0] */
const char movTrpFileName6[] = "MOV/TRP6.BIN";
const char movTrpFileName5[] = "MOV/TRP5.BIN";
const char movTrpFileName4[] = "MOV/TRP4.BIN";
const char movTrpFileName3[] = "MOV/TRP3.BIN";
const char movTrpFileName2[] = "MOV/TRP2.BIN";
const char movTrpFileName1[] = "MOV/TRP1.BIN";
const char movTrpFileName0[] = "MOV/TRP0.BIN";

/* videoCallback (4658A8.c) */
const char movStrPtsOverflow[] = "pts buffer overflow\n";

/* videoDecFlush (465E78.c) copies it as a 4-byte struct: the MPEG sequence
 * end code 00 00 01 B7 (probably a local array's initializer originally) */
const u8 movSeqEndCode[4] = { 0x00, 0x00, 0x01, 0xB7 };

/* decBs0 (466000.c) */
const char movStrDecodeError[] = "sceMpegGetPicture() decode error";
