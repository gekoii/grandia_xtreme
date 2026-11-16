/* FIELD .data: the field debug menu's names, tables and cursors, the map
 * HTA box test vectors, the polygon gather statistics and the equipment
 * slot tables. Data-only C (plan step 6b, docs/shifting.md); the users are
 * the debug pages (5D6140.c, 5D8DE0.c, 5DDEF0.c and their neighbours),
 * MapHtaHitBox, MapPolyClear/MapPolyGatherPart (5E5FA8.c, 5E6250.c) and
 * the equipment functions (5EB4F8.c, 5F1C68.c, 5F27C0.c).
 * Split from the task's 0x00666788-0x00666F90 at 0x00666810 and 0x00666CD0:
 * the zeros before both are what a 16-aligned file start recreates.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00666810-0x00666CD0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* .rodata: the OCC/OCCS status names */
extern char dbgFldOccStatusStrEnd[]; /* "End" */
extern char dbgFldOccStatusStrBackward[]; /* "Backward" */
extern char dbgFldOccStatusStrBackwardWait[]; /* "BackwardWait" */
extern char dbgFldOccStatusStrForward[]; /* "Forward" */
extern char dbgFldOccStatusStrBeginWait[]; /* "BeginWait" */
extern char dbgFldOccStatusStrFirstWait[]; /* "FirstWait" */
extern char dbgFldOccStatusStrWake[]; /* "Wake" */
extern char dbgFldOccStatusStrSleep[]; /* "Sleep" */

/* indexed by a flag */
char dbgFldOffOnStr[2][4] = { "OFF", "ON" }; /* 00666810 */

/* camera AI type names */
char dbgFldCamAiNames[14][16] = {
    "AI TYPE0",
    "GRANDIA2",
    "EYE",
    "LADDER",
    "FF",
    "TOWN",
    "TEST0",
    "TEST1",
    "TEST2",
    "TEST3",
    "TEST4",
    "TEST5",
    "FREE",
    "NULL ",
}; /* 00666818 */

/* character names by id (PARTY MEMBER LIST); dbgFldCharaMoveNames
 * (0x00666901) = dbgFldPartyNames[1], names by party slot (CHARA MOVE) */
char dbgFldPartyNames[10][9] = {
    "NONE",
    "EVAN",
    "BRANDOL",
    "CARMYNE",
    "JAID",
    "TITTO",
    "ULK",
    "MYAMM",
    "LUTINA",
    "HANIWA",
}; /* 006668F8 */

/* MAP JUMP: map numbers 0-0xB9 (the last two are 0) */
u16 dbgFldMapJumpTable[0xBA] = {
    0x2000, 0x2004, 0x2008, 0x200C, 0x2010, 0x2020, 0x2024, 0x2028,
    0x2030, 0x2034, 0x2038, 0x2040, 0x2050, 0x2060, 0x2070, 0x2074,
    0x2080, 0x2090, 0x2800, 0x2804, 0x3000, 0x3010, 0x3014, 0x3020,
    0x3030, 0x3034, 0x30A0, 0x30B0, 0x3400, 0x34A0, 0x3800, 0x3810,
    0x3820, 0x3830, 0x3838, 0x383C, 0x4000, 0x4004, 0x4010, 0x4014,
    0x4020, 0x4030, 0x4040, 0x4050, 0x40A0, 0x4800, 0x4810, 0x4820,
    0x48A0, 0x48F0, 0x5000, 0x5010, 0x5020, 0x5024, 0x5030, 0x50A0,
    0x50F0, 0x5800, 0x5804, 0x5810, 0x5820, 0x5824, 0x5830, 0x5840,
    0x58A0, 0x58F0, 0x5A00, 0x5A10, 0x5A20, 0x6000, 0x6004, 0x6008,
    0x600C, 0x6010, 0x6014, 0x6080, 0x6082, 0x6084, 0x6086, 0x6088,
    0x608A, 0x7000, 0x7010, 0x7020, 0x7024, 0x70A0, 0x70F0, 0x8000,
    0x8010, 0x8020, 0x80A0, 0x80A4, 0x80F0, 0x9000, 0x9010, 0x9014,
    0x9020, 0x9030, 0x9034, 0x9038, 0x9040, 0x9050, 0x9060, 0xA000,
    0xA004, 0xA008, 0xA00C, 0xA080, 0xA082, 0xA084, 0xA086, 0xD000,
    0xD010, 0xD020, 0xD400, 0xD800, 0xD804, 0xD808, 0xD80C, 0xD810,
    0xD814, 0xD818, 0xD81C, 0xD820, 0xD824, 0xD828, 0xD82C, 0xD830,
    0xD834, 0xD838, 0xD83C, 0xD840, 0xD844, 0xD848, 0xD84C, 0xD880,
    0xD882, 0xD884, 0xD886, 0xD888, 0xD88A, 0xD88C, 0xD88E, 0xD890,
    0xD892, 0xD894, 0xD896, 0xD898, 0xD89A, 0xD89C, 0xD89E, 0xD8A0,
    0xD8A2, 0xD8A4, 0xD8A6, 0xE000, 0xE010, 0xE020, 0xE030, 0xE040,
    0xE050, 0xE060, 0xE070, 0xE080, 0xE090, 0xE400, 0xE404, 0xE410,
    0xE420, 0xE424, 0xE428, 0xE430, 0xE434, 0xE438, 0xE440, 0xE450,
    0xE460, 0xE470, 0xE480, 0xE490, 0x0100, 0x0104, 0x1000, 0x1800,
}; /* 00666958 */

/* OCC/OCCS status names */
char *dbgFldOccStatusNames[8] = {
    dbgFldOccStatusStrSleep, dbgFldOccStatusStrWake, dbgFldOccStatusStrFirstWait, dbgFldOccStatusStrBeginWait,
    dbgFldOccStatusStrForward, dbgFldOccStatusStrBackwardWait, dbgFldOccStatusStrBackward, dbgFldOccStatusStrEnd,
}; /* 00666AD0 */

char dbgFldRunPauseStr[2][6] = { "RUN", "PAUSE" }; /* 00666AF0: CAMERA PAUSE */
char dbgFldStopRunStr[2][6] = { "STOP", "RUN" };   /* 00666B00: CAMERA STATUS */

/* page numbers, cursor lines and selections of the debug pages */
s32 dbgFldPartyMemberLine = 0;   /* 00666B0C: PARTY MEMBER LIST */
s32 dbgFldModelLine = 0;         /* 00666B10: MODEL */
s32 dbgFldModelTop = 0;          /* 00666B14 */
s32 dbgFldSystemLine = 0;        /* 00666B18: SYSTEM */
s32 dbgMovieNo = 0;              /* 00666B1C: movie to play (sysState.movieNo) */
s32 dbgFldSystemMenuPage = 0;    /* 00666B20: SYSTEM MENU page (0-2) */
s32 dbgFldCollisionLine = 0;     /* 00666B24: COLLISION */
s32 dbgFldCollisionTextX = 4;    /* 00666B28: text column of the collision vectors */
s32 dbgFldCollisionTextY = 6;    /* 00666B2C: text row */
s32 dbgFldLinesLine = 0;         /* 00666B30: LINE */
s32 dbgFldScenarioListLine = 0;  /* 00666B34: SCENARIO LIST */
s32 dbgFldScenarioListTop = 0;   /* 00666B38 */
s32 dbgFldScenarioMenuPage = 0;  /* 00666B3C */
s32 dbgFldChrMoveLine = 0;       /* 00666B40: CHARACTER MOVE DEBUG */
s32 dbgFldTenantNo = 0;          /* 00666B44: TENANT */
s32 dbgFldTenantPage = 0;        /* 00666B48: (0-5) */
s32 dbgFldPartyNo = 0;           /* 00666B4C: PARTY */
s32 dbgFldPartyPage = 0;         /* 00666B50: (0-5) */
s32 dbgFldMapJumpNo = 0;         /* 00666B54: MAP JUMP, index into dbgFldMapJumpTable */
s32 dbgFldMapJumpLine = 0;       /* 00666B58 */
s32 dbgFldMapJumpRepeat = 0;     /* 00666B5C: frames the value has been changing */
s32 dbgFldMainFlagLine = 0;      /* 00666B60: MAIN FLAG AND COUNTER */
s32 dbgFldFlagEditColumn = 0;    /* 00666B64: flag editor bit column (0-15) */
s32 dbgFldFlagEditTop = 0;       /* 00666B68 */
s32 dbgFldFlagEditLine = 0;      /* 00666B6C */
s32 dbgFldCounterEditColumn = 0; /* 00666B70: counter editor column (0-3) */
s32 dbgFldCounterEditLine = 0;   /* 00666B74 */
s32 dbgFldXvsInfoLine = 0;       /* 00666B78: XVS INFORMATION 1 */
s32 dbgFldXvsCharaLine = 0;      /* 00666B7C: XVS INFORMATION 2 */
s32 dbgFldGameDebugPage = 0;     /* 00666B80: GAME DEBUG (0-25) */
s32 dbgFldOccNo = 0;             /* 00666B84: OCC */
s32 dbgFldOccsNo = 0;            /* 00666B88: OCCS */
s32 dbgFldOccMenuPage = 0;       /* 00666B8C: OCC&OCS MENU */
s32 dbgFldCamScriptLine = 0;     /* 00666B90: CAMERA SCRIPT LIST */
s32 dbgFldCamScriptTop = 0;      /* 00666B94 */
s32 dbgFldCamInfoLine = 0;       /* 00666B98: camera script info */
s32 dbgFldCamTarget = 0;         /* 00666B9C: 0 player, n tenant n-1 */
s32 dbgFldCamScriptPage = 0;     /* 00666BA0: CAMERA SCRIPT (0-3) */
s32 dbgFldSetFileNo = 0;         /* 00666BA4: SETFILE monster entry */
s32 dbgFldMonsterTableNo = 0;    /* 00666BA8: MONSTER TABLE INF */
s32 dbgFldMonsterInfoPage = 0;   /* 00666BAC: MONSTER INFO (PARAM MENU) */
s32 dbgFldMonsterInfoPages = 0;  /* 00666BB0: its page count */

/* CHARA MOVE */
FVECTOR dbgFldMoveTargetPos = { 0.0f, 0.0f, 0.0f, 0.0f }; /* 00666BC0: "POS X/Y/Z" */
f32 dbgFldMoveDirect = 0.0f;  /* 00666BD0: direction in degrees */
u16 dbgFldMoveSpeed = 10;     /* 00666BD4 */
u8 dbgFldMoveKind = 0;        /* 00666BD6: 0 party, 1 tenant */
u8 dbgFldMoveIndex = 0;       /* 00666BD7: party slot or tenant */
f32 dbgFldMoveNeckX = 0.0f;   /* 00666BD8: neck angles (node 0x280) */
f32 dbgFldMoveNeckY = 0.0f;   /* 00666BDC */
f32 dbgFldMoveNeckZ = 0.0f;   /* 00666BE0 */
u8 dbgFldMoveCenterLine = 0;  /* 00666BE4: "CetnerLine" toggle */
s32 dbgFldMoveLine = 0;       /* 00666BE8 */

/* FADE */
f32 dbgFldFadeRed = 0.0f;    /* 00666BEC */
f32 dbgFldFadeGreen = 0.0f;  /* 00666BF0 */
f32 dbgFldFadeBlue = 0.0f;   /* 00666BF4 */
f32 dbgFldFadeAlpha = 0.0f;  /* 00666BF8 */
f32 dbgFldFadeTime = 60.0f;  /* 00666BFC */
s32 dbgFldFadeLine = 0;      /* 00666C00 */
s32 dbgFldFadeEditLine = 0;  /* 00666C04: last line edited */

/* MapHtaHitBox: the box's six face normals and its half size (a copy of
 * evtAreaBoxNormals/evtAreaBoxHalf) */
FVECTOR mapHtaBoxNormals[6] = {
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
}; /* 00666C10 */
FVECTOR mapHtaHalfVec = { 0.5f, 0.5f, 0.5f, 1.0f }; /* 00666C70 */

/* MapPolyGatherPart statistics (reset by MapPolyClear): triangles rejected
 * by each test, and the ones added to mapPolyList */
s32 mapPolyRejectBound = 0;  /* 00666C80: VuSphereTriBoundHit */
s32 mapPolyRejectPlane = 0;  /* 00666C84: VuTriPlaneNear */
s32 mapPolyRejectInside = 0; /* 00666C88: VuPointInTriangle */
s32 mapPolyAcceptCount = 0;  /* 00666C8C */
/* 0x20 zero bytes nothing refers to */
static s32 mapPolyUnused[8] = { 0 }; /* 00666C90 */

/* equipment: item type of each slot, and display slot -> PtyChar.equip index */
u8 itemEquipSlotType[6] = { 5, 6, 7, 8, 9, 10 };  /* 00666CB0 */
u8 itemEquipSlotOrder[6] = { 0, 1, 3, 2, 4, 5 }; /* 00666CB8 */
/* a table of 10 small numbers nothing refers to */
static u8 itemUnusedTable[10] = { 0, 0, 0, 0, 0, 1, 2, 2, 2, 6 }; /* 00666CC0 */
