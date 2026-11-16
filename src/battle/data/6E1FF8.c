/* BATTLE .rodata 0x006E1FF8-0x006E2080: the .rodata starts of four original
 * files, each from its "save": BtlDbgPrintBits (6C6CF0.c), one with nothing
 * else, then the unit state file (BtlUnitStateEscape 6CA3A8.c, the party
 * KO and get-up sounds of 6CABB0.c and 6CBB08.c, BtlUnitStateSetHandler
 * 6C8720.c). Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E1FF8-0x006E2080 */

typedef short s16;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* BtlDbgPrintBits: one character per bit */
const char btlDbgStrFmt[] = "%s";
const char btlDbgBitOnStr[] = "1";
const char btlDbgBitOffStr[] = "_";
static const char save_006E2018[] = "save"; /* the next file's .rodata starts here */
static const char save_006E2020[] = "save"; /* the next file's .rodata starts here */
/* BtlUnitStateEscape: font-coded captions (each character a byte, then
 * 0xF9) */
const char btlEscapeOkCaption[] = "q\371\224\371\215\371\237\371`\371\226\371\200\371\230\371@\371"; /* "Run Away!" */
const char btlEscapeFailCaption[] = "b\371\200\371\215\371F\371\223\371\237\371q\371\224\371\215\371"; /* "Can't Run" */
/* Sound effects by party character (character id - 0x1001): KO'd
 * (BtlUnitStatePartyKo), getting up 30 frames into the recovery from a
 * knock-down (BtlUnitStateHitRecover) */
const s16 btlPartyKoSeTable[8] = {
    0xFB3, 0xFC8, 0xFE2, 0xFFA, 0x1013, 0x102C, 0x1045, 0x105F,
};
const s16 btlPartyGetUpSeTable[8] = {
    0xFB2, 0xFC7, 0xFE1, 0xFF9, 0x1012, 0x102B, 0x1044, 0x105E,
};
/* The special-move tag (unit +0x94) of specials without one, and of
 * effect 0x5F */
const char btlMgcTagUnknown[] = "?";
