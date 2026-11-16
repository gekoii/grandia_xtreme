/* main executable: the start of a file's .rodata - the text glyph buffer's
 * overflow message (TxtGlyphBufPut 416F68). Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x004A2998-0x004A29C0 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* printf format when the glyph buffer is full (TxtGlyphBufPut), Shift-JIS
 * "文字バッファオーバーフロー\n" (character buffer overflow) */
const char txtGlyphOverflowMsg[] =
    "\225\266\216\232"                  /* 文字 */
    "\203\157\203\142\203\164\203\100"  /* バッファ */
    "\203\111\201\133\203\157\201\133"  /* オーバー */
    "\203\164\203\215\201\133"          /* フロー */
    "\n";
