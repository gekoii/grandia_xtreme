/* main executable .data 0x00499D80-0x0049A660: particle debug state, the
 * shatter effect's light block, the GRX.INI key table, the menu widgets'
 * text colour, window frame and number fields, the common model groups and
 * the attract demo maps. Data-only C (plan step 6b, docs/shifting.md); the
 * users are 402590.c/4028D8.c (particles), 40C7B0.c (GphShatterLoad),
 * 40D860.c (SysIniLoad), 417DE8.c and 418390.c (menu widgets), 41AF50.c and
 * 41CDB8.c (help window), 41E1D8.c and 41E278.c (system data, demo).
 * The debug pages' data that follows (from 0x0049A660) is 49A660.c: that
 * original file's .data is 16-aligned (dbgEnvMapParam), which leaves the gap
 * after sysDemoMapIds.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00499D80-0x0049A660 */

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* ---- particle debug (ptcl.h) ---- */
s32 ptclStarterFrame = -1;
s16 ptclStartMaskId = -1;       /* -1 = none */
f32 ptclStartOffsetY = 0.0f;
s32 ptclDbgExtFileSize = 0;     /* set by 40B938 */
s32 ptclDbgExtSortSize = 0;
s32 ptclDbgExtInstSize = 0;
s32 dbgPtclCtrlIndex = 0;       /* DbgPtclCtrlPage (4032B8) */
s32 dbgPtclCtrlGenIndex = 0;    /* 4047B8 */
s32 dbgPtclGenIndex = 0;        /* DbgPtclGenPage (407848) */
s32 dbgPtclEffectorIndex = 0;   /* 40B018 */

/* ---- shatter effect (gph.h) ---- */
/* VU light block (GphLightVuBlock, GphLightVuBlockReset 432018), 16-aligned
 * by its vectors: GphShatterLoad (40C7B0) points every GA object's
 * lightBlock at it, so the shatter pieces are drawn with no lights. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u32 vifCode;                /* VIF UNPACK V4-32 */
    FVECTOR unk10[8];
    FVECTOR dirColor[4];
    FVECTOR pointColor[4];
    FVECTOR dirVec[4];
    FVECTOR pointPos[4];
} GphLightVuBlock; /* size 0x190 */

GphLightVuBlock gphShatterLightBlock = { 0, 0, 0, 0x6C100180 };

/* ---- GRX.INI (sys.h) ---- */
/* SysIniLoad (40D860) matches each line's first 10 characters against these
 * keys (no NUL); the value goes to sysIni (or sysState for the first four). */
char sysIniKeyTable[150][10] = {
    "program no", "debug mode", "frame mode", "frame skip", "backcol  r",
    "backcol  g", "backcol  b", "fogcolor r", "fogcolor g", "fogcolor b",
    "fog   dist", "clipping f", "clipping n", "view angle", "ambient  r",
    "ambient  g", "ambient  b", "ambient  a", "light0   r", "light0   g",
    "light0   b", "light0   a", "light0   x", "light0   y", "light1   r",
    "light1   g", "light1   b", "light1   a", "light1   x", "light1   y",
    "motion spd", "motion smt", "collision0", "viewer lgt", "viewer crs",
    "viewer sdw", "viewer stg", "ground0dsp", "viewer lop", "viewer ptc",
    "walkspeed0", "usemdtdata", "camera_ai0", "encount_on", "pad_vibrat",
    "map_number", "map_warpno", "viewer mip", "item_no_00", "item_no_01",
    "item_no_02", "item_no_03", "item_no_04", "item_no_05", "item_no_06",
    "item_no_07", "item_no_08", "item_no_09", "item_num00", "item_num01",
    "item_num02", "item_num03", "item_num04", "item_num05", "item_num06",
    "item_num07", "item_num08", "item_num09", "party_ext0", "party_ext1",
    "party_ext2", "party_ext3", "party_ext4", "party_ext5", "party_ext6",
    "party_ext7", "party0_num", "party0_exp", "party0_eq0", "party0_eq1",
    "party0_eq2", "party0_eq3", "party0_eq4", "party0_eq5", "party0_sl0",
    "party0_sl1", "party0_sl2", "party0_sl3", "party0_sl4", "party0_sl5",
    "mons_00_no", "mons_01_no", "mons_02_no", "mons_03_no", "mons_00num",
    "mons_01num", "mons_02num", "mons_03num", "mons_00_lv", "mons_01_lv",
    "mons_02_lv", "mons_03_lv", "start_scnr", "party1_num", "party1_exp",
    "party1_eq0", "party1_eq1", "party1_eq2", "party1_eq3", "party1_eq4",
    "party1_eq5", "party1_sl0", "party1_sl1", "party1_sl2", "party1_sl3",
    "party1_sl4", "party1_sl5", "party2_num", "party2_exp", "party2_eq0",
    "party2_eq1", "party2_eq2", "party2_eq3", "party2_eq4", "party2_eq5",
    "party2_sl0", "party2_sl1", "party2_sl2", "party2_sl3", "party2_sl4",
    "party2_sl5", "party3_num", "party3_exp", "party3_eq0", "party3_eq1",
    "party3_eq2", "party3_eq3", "party3_eq4", "party3_eq5", "party3_sl0",
    "party3_sl1", "party3_sl2", "party3_sl3", "party3_sl4", "party3_sl5",
    "mainflagno", "mapse_bnk0", "mapse_bnk1", "mapse_bnk2", "mapse_bnk3",
};

/* ---- party (party.h) ---- */
f32 ptyLevelDownExpRate = 0.0f; /* 4166E8 */

/* ---- menu widgets (main/n02.h) ---- */
/* Colour/offset for the next MenuTextDraw (reset after each draw) */
u8 menuTextAddR = 0;            /* added colour */
u8 menuTextAddG = 0;
u8 menuTextAddB = 0;
u8 menuTextGradR = 0x80;        /* bottom colour with TXT_STYLE_GRADIENT */
u8 menuTextGradGValue = 0x80;
u8 menuTextGradB = 0x80;
u8 menuTextOffsetX = 4;         /* skew with TXT_STYLE_SKEW */
u8 menuTextOffsetY = 0;

/* Texture rectangle of a sprite (sys.h): size, then the UV corners */
typedef struct {
    s16 w;
    s16 h;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} SprRect; /* size 0xC */

/* Window frame pieces drawn by MenuFrameDraw (418390) around a MenuFrame,
 * texture 0xED; the edges' w (and the sides' h) are set before each draw.
 * fields: D_0049A540 [2], D_0049A588 [8] */
SprRect menuFrameParts[11] = {
    { 10, 24, 0xD5, 0x21, 0xE0, 0x3A },     /* top left corner */
    { 8, 10, 0xE0, 0x21, 0xE7, 0x2C },      /* top edge, left of the centre */
    { 8, 10, 0xE7, 0x21, 0xF0, 0x2C },      /* top centre */
    { 8, 10, 0xF0, 0x21, 0xF7, 0x2C },      /* top edge, right of the centre */
    { 8, 24, 0xF7, 0x21, 0x100, 0x3A },     /* top right corner */
    { 6, 70, 0xCD, 0x02, 0xD4, 0x49 },      /* side edge (drawn left and right) */
    { 10, 10, 0xD5, 0x3D, 0xE0, 0x48 },     /* bottom left corner */
    { 8, 10, 0xE0, 0x3D, 0xE7, 0x48 },      /* bottom edge, left */
    { 8, 10, 0xE7, 0x3D, 0xF0, 0x48 },      /* bottom centre */
    { 8, 10, 0xF0, 0x3D, 0xF7, 0x48 },      /* bottom edge, right */
    { 8, 10, 0xF7, 0x3D, 0x100, 0x48 },     /* bottom right corner */
};

/* Number field (sys.h, MenuNumberLayout 417E80) */
typedef struct {
    void *font;                 /* SprDigitFont * */
    void *sprites;              /* output list, set by MenuNumberDraw */
    f32 sx;                     /* scale */
    f32 sy;
    s16 x;
    s16 y;
    s8 digits;
    u8 pad15[3];
} MenuNumField; /* size 0x18 */

extern u8 menuDigitFont[];      /* SprDigitFont */

MenuNumField menuHelpStatNumField = { menuDigitFont, 0, 0.0f, 0.0f, 0, 0, 3 }; /* 41CDB8 */
MenuNumField menuHelpLinkNumField = { menuDigitFont, 0, 1.0f, 0.0f, 0, 0, 3 }; /* linked skill level */

/* ---- system data (main/n02.h, sys.h) ---- */
/* Model group per common model id, -1 = 0 (SysCommonModelGet 41E1D8) */
s8 sysCommonModelGroups[100] = {
    -1, 7, 7, 7, 7, 7, 34, 34, 34, 34, 34, 34, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, 22, 23, 24, 25, 26, 27, 28, 29, 20, 20, 20,
    20, 20, 30, 30, 31, 5, 31, 19, 19, 17, 17, 17, 18, 18, 18, 18,
    6, 8, 10, 10, 10, 10, 13, 11, 11, 15, 11, 11, 32, 16, 12, 14,
    7, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 32, 21, 33, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, -1, -1,
    -1, -1, -1, 33,
};

/* Maps of the attract demo (41E278) */
u16 sysDemoMapIds[6] = { 0x5010, 0x4050, 0x4820, 0x5830, 0x4810, 0x5020 };
