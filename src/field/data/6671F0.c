/* FIELD .data: the event character select tables (formations and cameras),
 * the mini-map ids and the auto-map generator's constants. Data-only C
 * (plan step 6b, docs/shifting.md); the users are EvtCharSelStart
 * (5FE710.c), MapMiniIndexGet (5FFA58.c), AmapGenerate (602CC0.c) and
 * AmapSetupFloorParams (614628.c).
 * Split from the task's 0x00666F90-0x006673C8 at 0x006671F0.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x006671F0-0x006673C8 */

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* EvtCharSelStart: tenant ids of the 9 selectable characters */
u16 evtCharSelTenantIds[9] = { 2, 3, 4, 5, 6, 7, 8, 9, 10 }; /* 006671F0 */

/* camera positions for formations of 2, 3, 4 and 5 characters */
FVECTOR evtCharSelCam2[7] = {
    { 527.0f, 500.0f, 114.0f, 0.0f },
    { 650.0f, 544.0f, -639.0f, 0.0f },
    { 273.0f, 486.0f, -977.0f, 0.0f },
    { -385.0f, 442.0f, -407.0f, 0.0f },
    { 527.0f, 434.0f, -904.0f, 0.0f },
    { 527.0f, 434.0f, -904.0f, 0.0f },
    { 527.0f, 434.0f, -904.0f, 0.0f },
}; /* 00667210 */
FVECTOR evtCharSelCam3[3] = {
    { 655.0f, 596.0f, 177.0f, 0.0f },
    { 472.0f, 534.0f, -673.0f, 0.0f },
    { -162.0f, 536.0f, -675.0f, 0.0f },
}; /* 00667280 */
FVECTOR evtCharSelCam4[2] = {
    { 713.0f, 463.0f, -600.0f, 0.0f },
    { -270.0f, 412.0f, -791.0f, 0.0f },
}; /* 006672B0 */
FVECTOR evtCharSelCam5[3] = {
    { 640.0f, 589.0f, -970.0f, 0.0f },
    { 689.0f, 195.0f, -832.0f, 0.0f },
    { 394.0f, 347.0f, -593.0f, 0.0f },
}; /* 006672D0 */

/* formation slots for 2, 3 and 4 characters */
s8 evtCharSelPlace2[7][2] = {
    { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 }, { 0, 2 }, { 1, 3 }, { 2, 4 },
}; /* 00667300 */
s8 evtCharSelPlace3[3][3] = {
    { 0, 1, 2 }, { 1, 2, 3 }, { 2, 3, 4 },
}; /* 00667310 */
s8 evtCharSelPlace4[2][4] = {
    { 0, 1, 2, 3 }, { 1, 2, 3, 4 },
}; /* 00667320 */

/* MapMiniIndexGet: map id by mini-map index (-1 none) */
u16 mapMiniMapIds[0x30] = {
    0x4000, 0x4030, 0x4040, 0x4050, 0x4800, 0x4810, 0x4810, 0x4820,
    0x4820, 0x48F0, 0x5010, 0x5020, 0x5024, 0x5030, 0x5030, 0x50F0,
    0x5810, 0x5020, 0x5030, 0x5800, 0x5804, 0x5810, 0x5820, 0x5820,
    0x5820, 0xFFFF, 0x5830, 0x5830, 0x5840, 0x58F0, 0x7000, 0x7010,
    0x7020, 0x7020, 0x7024, 0x70F0, 0x8000, 0x8010, 0x8020, 0x80F0,
    0x9000, 0x9010, 0x9014, 0xFFFF, 0x9020, 0x9034, 0x9040, 0xFFFF,
}; /* 00667328 */

/* AmapSetupFloorParams: copied to the generator work +0x2A/+0x30, and one
 * picked at random for +0x34 */
u16 amapFloorFixedParams[2] = { 1, 0x78 };     /* 00667388 */
u8 amapFloorRandParams[4] = { 0, 4, 8, 16 };   /* 00667390 */

/* AmapGenerate: 2 bytes per map type, the first copied to the generator
 * header +0x21 (set for types 0 and 3-7) */
u8 amapTypeFlagTable[24][2] = {
    { 1, 0 }, { 0, 0 }, { 0, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 },
}; /* 00667398 */
