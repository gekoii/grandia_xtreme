/* main executable .data 0x0049A660-0x0049A848: the debug pages' state
 * (camera, load meter, environment, object, frame buffer, pad, sound,
 * motion, wipe, fade, program call, attribute and texture pages). Data-only
 * C (plan step 6b, docs/shifting.md); the users are 420778.c..42C330 (the
 * debug pages, main/n02.h). The file's .rodata starts at 0x004A36A0 ("save",
 * 4A3640.c), where dbgCamModeNames' strings are.
 * dbgEnvMapParam is a 16-aligned vector, so this file's .data is 16-aligned:
 * that gives the gap before dbgCamModes (after sysDemoMapIds, 499D80.c) and
 * the one after dbgClearB.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x0049A660-0x0049A848 */

typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

/* Strings in this file's .rodata (4A3640.c) */
extern char dbgCamModeStrPosEyeVec[];       /* " pos    + eye vector + dist" */
extern char dbgCamModeStrPosWangle[];       /* " pos    + wangle + dist" */
extern char dbgCamModeStrTargetEyeVec[];       /* " target + eye vector + dist" */
extern char dbgCamModeStrTargetPos[];       /* " target + pos + dist " */
extern char dbgCamModeStrTargetWangle[];       /* " target + wangle + dist" */

/* CAMERA page (DbgCameraPage 420C78): camera modes it cycles through */
s32 dbgCamModes[5] = { 0x39, 0x33, 0x35, 0x3A, 0x36 };

s8 dbgObjNo = 0;                /* object selected on the debug pages (DbgGetObjNo) */
u8 dbgClearR = 0x40;            /* clear colour (DbgSetClearColor 420778) */
u8 dbgClearG = 0x40;
u8 dbgClearB = 0x40;

/* sceVu0FVECTOR-like colour (main/n02.h) */
typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} __attribute__((aligned(16))) DbgColor; /* size 0x10 */

/* env-map colour; a = strength (GphEnvMapSetColor). fields: D_0049A68C .a */
DbgColor dbgEnvMapParam = { 128.0f, 128.0f, 128.0f, 0.0f };

/* names of dbgCamModes[] */
char *dbgCamModeNames[5] = {
    dbgCamModeStrTargetWangle, dbgCamModeStrTargetPos, dbgCamModeStrTargetEyeVec, dbgCamModeStrPosWangle, dbgCamModeStrPosEyeVec,
};

/* load meter (DbgLoadMeterDraw 420990) */
s32 dbgLoadFrames = 0;
f32 dbgLoadMaxPrev = 0.0f;
f32 dbgLoadMinPrev = 0.0f;
f32 dbgLoadMax = 0.0f;
f32 dbgLoadMin = 99999.0f;

/* CAMERA page */
u32 dbgCamLine = 0;
s32 dbgCamCursor = 0;
s32 dbgCamModeNo = 1;

/* ENVIROMENTS page */
s32 dbgEnvLine = 0;
s32 dbgEnvLightNo = 0;
s32 dbgEnvCursor = 0;

/* OBJECT page */
s32 dbgObjLine = 0;
s32 dbgObjCursor = 0;
s32 dbgObjObjNo = 0;
s32 dbgObjPartsNo = 0;
s32 dbgObjGaObjNo = 0;

/* frame buffer pages */
u32 dbgFbDivLine = 0;
u32 dbgFbDivCursor = 0;
s32 dbgFbEffLine = 0;
s32 dbgFbEffCursor = 0;
u32 dbgFbSys2Line = 0;
u32 dbgFbSys2Cursor = 0;
s32 dbgFbPage = 0;

/* PAD page (DbgPadPage 424E90): vibration motors */
u8 dbgPadMotorLeft = 0;
u8 dbgPadMotorRight = 0;

s32 dbgFilePage = 0;

/* sound pages (425808 BGM, 425DB0 SE) */
s32 dbgSndBgmNo = 0;
s32 dbgSndBgmLine = 0;
s32 dbgSndBgmCursor = 0;
u8 dbgSndBgmFade = 0x80;
f32 dbgSndBgmVolume = 100.0f;
s32 dbgSndSeNo = 2000;
s32 dbgSndSeLine = 0;
s32 dbgSndSeCursor = 0;
s32 dbgSndSeHpan = 0;
s32 dbgSndEqChannel = 1;
s32 dbgSndEqTimer = 3;
s32 dbgSndPage = 0;

/* MOTION page */
s32 dbgMotLine = 0;
s32 dbgMotCursor = 0;
s32 dbgMotMotionNo = 0;
s8 dbgMotPartsSel = 0;
s32 dbgMotPartsTop = 0;
s32 dbgMotPartsCursor = 0;
s32 dbgMotPage = 0;

/* screen division page */
s32 dbgScrDivLine = 0;
s32 dbgScrDivCursor = 0;

/* point light page */
u32 dbgPtLightLine = 0;
s32 dbgPtLightCursor = 0;
s32 dbgPtLightNo = 0;

/* wipe page */
s32 dbgWipeType = 0;
s32 dbgWipeParam0 = 0;
s32 dbgWipeParam1 = 0;
s32 dbgWipeLine = 0;
s32 dbgWipeCursor = 0;

/* fade page */
f32 dbgScreenFadeRed = 0.0f;
f32 dbgScreenFadeGreen = 0.0f;
f32 dbgScreenFadeBlue = 0.0f;
f32 dbgScreenFadeAlpha = 0.0f;
f32 dbgScreenFadeTime = 60.0f;
u32 dbgFadeLine = 0;
s32 dbgFadeCursor = 0;

/* SYSTEM page */
s32 dbgSysCursor = 0;
s32 dbgSysLine = 0;
s32 dbgMemPartition = 0;

/* program call page (DbgProgramCallPage 4291D8) */
s32 dbgProgLine = 0;
s32 dbgProgCursor = 0;
char sysProgramNames[7][16] = {
    "VIEWER", "TITLE", "FIELD", "BATTLE", "ENDING", "MCCHECK", "AUTOMAP",
};

/* ATR pages */
s32 dbgAtrPage = 0;
s32 dbgAtrObjNo = 0;
s32 dbgObjAtrLine = 0;
s32 dbgObjAtrCursor = 0;
s32 dbgPartsAtrLine = 0;
s32 dbgPartsAtrCursor = 0;
s32 dbgPartsAtrPartNo = 0;

/* texture page */
u32 dbgTexNo = 0;
s32 dbgTexLine = 0;
s32 dbgTexCursor = 0;
