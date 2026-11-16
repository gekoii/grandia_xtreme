/* FIELD: the start of .data - the event area (HTA) module's variables and
 * tables. Data-only C (plan step 6b, docs/shifting.md); the functions that
 * use them are EvtAreaInit/EvtAreaAlloc/EvtAreaUpdate (5B2208.c-5B2418.c),
 * EvtAreaLoad (5B3A18.c) and EvtAreaHitBox (asm/field/field/002BC0.s).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00666080-0x00666160 */

typedef signed int s32;
typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16)));

void *evtAreaTable = 0;     /* 00666080: {count, 0x100-byte entries} */
s32 evtAreaUse = 1;         /* 00666084: "HTA USE" on the debug SYSTEM page; EvtAreaUpdate does nothing while 0 */
s32 evtAreaFrameWorkA = 0;  /* 00666088: zeroed by EvtAreaAlloc and every EvtAreaUpdate, never read */
s32 evtAreaFrameWorkB = 0;  /* 0066608C: zeroed with evtAreaFrameWorkA, never read */

/* EvtAreaLoad maps the script's codes to the work's values */
s32 evtAreaShapeTable[4] = { 0, 1, 2, 3 };                       /* 00666090: by type -> shape */
s32 evtAreaTargetMaskTable[6] = { 1, 6, 7, 8, 16, 0 };           /* 006660A0: -> chk (who triggers it) */
s32 evtAreaAttrTable[4] = { 0, 1, 2, 0 };                        /* 006660B8 */
s32 evtAreaFireCondTable[10] = { 0, 1, 2, 3, 4, 0, 0, 0, 0, 0 }; /* 006660C8: -> mode */

/* EvtAreaHitBox: the box's six face normals and its half size */
FVECTOR evtAreaBoxNormals[6] = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
}; /* 006660F0 */
FVECTOR evtAreaBoxHalf = { 0.5f, 0.5f, 0.5f, 1.0f }; /* 00666150 */
