/* BATTLE .rodata: the start of a file's .rodata (its "save" string), the
 * battle command list's captions. Data-only C (plan step 6b,
 * docs/shifting.md). The text is in the game's font code: each character
 * is a byte (ASCII + 0x1F, digits too) followed by 0xF9; the comments give
 * it as text. Strings packed at a fixed stride are 2D char arrays (gcc 2.9
 * aligns every array to 8, so packed strings can't be arrays of their own).
 * Don't include the project headers here.
 */
/* rodata: 0x006E0198-0x006E0320 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* Target area names by target type 1-7. The code indexes them through
 * btlTargetAreaNames (0x006E0193), a label 13 bytes before the table: the
 * name of type 0, which doesn't exist (config/symbol_exprs.ld). */
const char btlTargetAreaNameText[7][13] = {
    "r\371\210\371\215\371\206\371\213\371\204\371",
    "b\371\210\371\221\371\202\371\213\371\204\371",
    "k\371\210\371\215\371\204\371",
    "e\371\200\371\215\371",
    "`\371\213\371\213\371",
    "`\371\221\371\216\371\224\371\215\371\203\371",
    "t\371\215\371\210\371\220\371\224\371\204\371",
}; /* 006E01A0 */ /* "Single" "Circle" "Line" "Fan" "All" "Around" "Unique" */
/* Equipment slot names, one string after another; .data points at each
 * (btlCmdListEquipSlotNames, btlCmdListEquipSlotNameText + 0xD, ...) */
const char btlCmdListEquipSlotNameText[] = "v\371\204\371\200\371\217\371\216\371\215\371\000a\371\216\371\203\371\230\371\000`\371\221\371\214\371\000g\371\204\371\200\371\203\371\000k\371\204\371\206\371\000`\371\202\371\202\371\222\371"; /* 006E0200 */ /* "Weapon" "Body" "Arm" "Head" "Leg" "Accs" */
/* btlStrSp is btlStrMp[1] */
const char btlStrMp[2][5] = {
    "l\371o\371",
    "r\371o\371",
}; /* 006E0238; fields: btlStrSp +0x5 */ /* "MP" "SP" */
/* Item and move list page titles */
const char btlCmdListItemPageNames[6][13] = {
    "d\371\220\371\224\371\210\371\217\371\222\371",
    "h\371\223\371\204\371\214\371\237\371P\371",
    "h\371\223\371\204\371\214\371\237\371Q\371",
    "h\371\223\371\204\371\214\371\237\371R\371",
    "h\371\223\371\204\371\214\371\237\371S\371",
    "h\371\223\371\204\371\214\371\237\371T\371",
}; /* 006E0248 */ /* "Equips" "Item 1" "Item 2" "Item 3" "Item 4" "Item 5" */
const char btlCmdListMovePageNames[6][13] = {
    "l\371\216\371\225\371\204\371\222\371",
    "e\371\204\371\200\371\223\371\237\371P\371",
    "e\371\204\371\200\371\223\371\237\371Q\371",
    "e\371\204\371\200\371\223\371\237\371R\371",
    "e\371\204\371\200\371\223\371\237\371S\371",
    "e\371\204\371\200\371\223\371\237\371T\371",
}; /* 006E0298 */ /* "Moves" "Feat 1" "Feat 2" "Feat 3" "Feat 4" "Feat 5" */
/* Shown for an empty equipment / move slot */
const char btlCmdListStrNoEquip[] = "L\371L\371L\371L\371L\371L\371L\371L\371L\371L\371L\371L\371"; /* 006E02E8 */ /* "------------" */
const char btlCmdListStrNoMove[] = "L\371L\371L\371L\371L\371L\371L\371L\371L\371"; /* 006E0308 */ /* "---------" */
