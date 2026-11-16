/* FIELD .data 0x006694E0-0x006697B0: data-only C (plan step 6b, docs/shifting.md).
 * Auto-map dungeon table (random dungeon parameters by map).
 */
/* data: 0x006694E0-0x006697B0 */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Dungeon table entry (AmapSetupFloorParams, AmapCalcFloorNo); the same
 * struct as AmapDungeon in include/field/n22.h. */
typedef struct AmapDungeon {
    /* 0x00 */ u16 mapId;
    /* 0x02 */ u16 lastFloor;      /* compared with the exit object's +0x38 */
    /* 0x04 */ u16 resetFlag;      /* exit resets amapFloorIndex */
    /* 0x06 */ u16 exitMap;        /* AmapGetExitDest (| 0x40000000) */
    /* 0x08 */ u16 firstFloorNo;   /* floor banner number of floor 0 */
    /* 0x0A */ u16 spotChance;     /* % chance of the item spots */
    /* 0x0C */ u16 gridSize;       /* AmapWork +0x3B */
    /* 0x0E */ u16 unkE;           /* AmapWork +0x110 */
    /* 0x10 */ u16 unk10;          /* AmapWork +0x112 */
    /* 0x12 */ u16 unk12;          /* AmapWork +0x114 (void grow rate) */
    /* 0x14 */ u16 unk14;          /* AmapWork +0x116 */
    /* 0x16 */ u16 unk16;          /* AmapWork +0x118 (void fill mode) */
} AmapDungeon; /* size 0x18 */

AmapDungeon amapDungeonTable[30] = {
    /* mapId  last reset exitMap first spot% grid unkE unk10 unk12 unk14 unk16 */
    { 0x6000, 4, 1, 0x6080, 1, 25, 28, 0, 0, 0, 1, 1 },
    { 0x6004, 4, 1, 0x6082, 6, 25, 28, 0, 0, 0, 1, 1 },
    { 0x6008, 4, 1, 0x6084, 11, 25, 28, 1, 0, 0, 1, 1 },
    { 0x600C, 4, 1, 0x6086, 16, 25, 28, 0, 1, 0, 1, 1 },
    { 0x6010, 4, 1, 0x6088, 21, 25, 28, 1, 1, 0, 1, 1 },
    { 0x6014, 4, 1, 0x608A, 26, 25, 28, 1, 1, 0, 1, 1 },
    { 0xA000, 4, 1, 0xA080, 1, 25, 28, 1, 1, 0, 0, 1 },
    { 0xA004, 4, 1, 0xA082, 6, 25, 28, 1, 1, 20, 0, 1 },
    { 0xA008, 2, 0, 0xA084, 11, 25, 30, 0, 0, 0, 0, 0 },
    { 0xA008, 4, 1, 0xA086, 14, 25, 30, 0, 0, 0, 0, 0 },
    { 0xD800, 4, 1, 0xD880, 1, 10, 28, 1, 1, 0, 0, 1 },
    { 0xD804, 4, 1, 0xD882, 6, 40, 28, 1, 1, 20, 0, 1 },
    { 0xD808, 4, 1, 0xD884, 11, 10, 28, 1, 1, 0, 0, 1 },
    { 0xD80C, 4, 1, 0xD886, 16, 10, 28, 1, 1, 20, 0, 1 },
    { 0xD810, 4, 1, 0xD888, 21, 10, 28, 1, 1, 0, 1, 1 },
    { 0xD814, 4, 1, 0xD88A, 26, 40, 28, 1, 1, 20, 1, 1 },
    { 0xD818, 4, 1, 0xD88C, 31, 10, 28, 1, 1, 0, 1, 1 },
    { 0xD81C, 4, 1, 0xD88E, 36, 10, 28, 1, 1, 20, 1, 1 },
    { 0xD820, 4, 1, 0xD890, 41, 10, 28, 1, 1, 0, 0, 0 },
    { 0xD824, 4, 1, 0xD892, 46, 40, 28, 1, 1, 0, 0, 0 },
    { 0xD828, 4, 1, 0xD894, 51, 0, 28, 1, 1, 0, 0, 0 },
    { 0xD82C, 4, 1, 0xD896, 56, 10, 28, 1, 1, 0, 0, 0 },
    { 0xD830, 4, 1, 0xD898, 61, 10, 28, 1, 0, 0, 0, 2 },
    { 0xD834, 4, 1, 0xD89A, 66, 70, 28, 1, 0, 0, 0, 1 },
    { 0xD838, 4, 1, 0xD89C, 71, 10, 28, 1, 0, 0, 0, 2 },
    { 0xD83C, 4, 1, 0xD89E, 76, 10, 28, 1, 0, 0, 0, 1 },
    { 0xD840, 4, 1, 0xD8A0, 81, 10, 28, 1, 1, 0, 0, 1 },
    { 0xD844, 4, 1, 0xD8A2, 86, 40, 28, 1, 1, 0, 0, 1 },
    { 0xD848, 4, 1, 0xD8A4, 91, 0, 28, 1, 1, 100, 0, 0 },
    { 0xD84C, 4, 1, 0xD8A6, 96, 0, 28, 1, 1, 100, 0, 0 },
};
