/* main executable .data 0x0049AD80-0x0049B000: the particle emitters' strip
 * index table and colour packet header. Data-only C (plan step 6b,
 * docs/shifting.md); the users are PtclEmitterSysInit (450648.c) and
 * PtclEmitterReset (450FB0.c). The same tables for the plain particle strip
 * are built at run time by PtclInit (44A1F8.c: ptclStripIndices, ...).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049AD80-0x0049B000 */

typedef unsigned int u32;

/* Two of the index pairs (64, 80) and (176, 80) read as .bss addresses, so
 * config/reloc_addrs.txt tells splat they are numbers (MIPS_NONE). */

/* Two s16 entries in a word, low first */
#define P(lo, hi) ((lo) | (hi) << 16)
/* Quad i of the strip: the qword offsets of its vertices 4i..4i+3
 * (64i + 0/16/32/48), each triangle followed by 32i or 32i + 16 (the
 * same pattern PtclInit writes, 44A1F8.c) */
#define ROW(i) \
    P((i) * 64, (i) * 64 + 16), P((i) * 64 + 32, (i) * 32), \
    P((i) * 64, (i) * 64 + 32), P((i) * 64 + 48, (i) * 32 + 16)

/* Emitter draw template's index list (s16[256]): 31 quads, then zeros */
u32 ptclEmitterStripIndices[128] = {
    ROW(0),
    P(64, 80), P(96, 32), P(64, 96), P(112, 48),
    P(128, 144), P(160, 64), P(128, 160), P(176, 80),
    ROW(3),
    ROW(4),
    ROW(5),
    ROW(6),
    ROW(7),
    ROW(8),
    ROW(9),
    ROW(10),
    ROW(11),
    ROW(12),
    ROW(13),
    ROW(14),
    ROW(15),
    ROW(16),
    ROW(17),
    ROW(18),
    ROW(19),
    ROW(20),
    ROW(21),
    ROW(22),
    ROW(23),
    ROW(24),
    ROW(25),
    ROW(26),
    ROW(27),
    ROW(28),
    ROW(29),
    ROW(30),
};

/* Header of each emitter's colour packet (PtclEmitterReset copies the first
 * 4 qwords): VIF UNPACK V4-8 of 0x7C colours to VU address 0x100 */
u32 ptclEmitterColorHeader[32] = { 0, 0, 0, 0x6E7C0100 };
