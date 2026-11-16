/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: the .rodata of newlib's vfscanf.c (__svfscanf,
 * 485E38.c): basefix, the function's local static table (in .rodata, so
 * const here; newlib's isn't). Data-only C (plan step 6b,
 * docs/shifting.md). vfscanf.o's .rodata continues with the switch's jump
 * table, 16-aligned at 0x004A8760 (485E38.c's own .rodata), so the zeros
 * after basefix are that alignment's. Don't include the project headers
 * here.
 */
/* rodata: 0x004A8728-0x004A8760 */

/* 8 zero bytes before basefix: no label and no users; most likely the end
 * of the object before (vfprintf.c's .rodata ends at 0x004A8724, the 4 zero
 * bytes after it are an asm piece) */
static const unsigned int zero_4A8728[2] = { 0, 0 };

/* base for a conversion: basefix[0] = 10, basefix[n] = n */
const short basefix[17] = {
    10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
};
