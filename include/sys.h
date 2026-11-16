#ifndef SYS_H
#define SYS_H

/*
 * System state of the main executable: sysState (boot/main loop, heaps,
 * active programs, debug pages, the three system font images), the GRX.INI
 * settings, the program table, the attract demo, the 2D sprite state and the
 * field <-> battle hand-off block.
 *
 * Several globals below are separate linker symbols that point INTO one of
 * the structs (sysLoopFrameCount = &sysState.loopFrameCount, ...). They keep
 * their own externs so files can go on using them.
 */

#include "common.h"
#include "gph.h"    /* GsTexPacket */

/* ------------------------------------------------------------------------ */
/* Font image (0x680): a glyph texture filled by TxtFontImagePutGlyph16/18.  */
/* sysState holds three at +0x80; FIELD's fldState holds three more.          */
/* ------------------------------------------------------------------------ */
#ifndef TXT_FONT_IMAGE_DEFINED
#define TXT_FONT_IMAGE_DEFINED
typedef struct TxtFontImage {
    /* 0x000 */ s32 w;
    /* 0x004 */ s32 h;
    /* 0x008 */ s32 cellW;           /* glyph cell size (16 or 18) */
    /* 0x00C */ s32 cellH;
    /* 0x010 */ u8 *pixels;          /* pixel buffer, cleared by TxtFontImageClear (passed as s32 by
                                      * the setup functions' local prototypes: casts there) */
    /* 0x014 */ s32 pixelSize;       /* bytes, w / 2 * h */
    /* 0x018 */ u8 *glyphSrc;        /* glyph bitmaps the PutGlyph functions copy from */
    /* 0x01C */ u8 *glyphMetricsSrc; /* GRXSYS +0xAC glyph table (SysDataInit): 2 bytes per glyph,
                                      * copied into cellMetrics by the PutGlyph functions */
    /* 0x020 */ u8 cellMetrics[16][16][2]; /* per cell: TxtFontImageGetGlyphMetrics */
    /* 0x220 */ u8 pad220[0x480 - 0x220];
    /* 0x480 */ s32 texDim[2];       /* GsTexInfoInit output: w, h */
    /* 0x488 */ u8 pad488[0x49C - 0x488];
    /* 0x49C */ s32 unk49C;
    /* 0x4A0 */ u8 pad4A0[0x500 - 0x4A0];
    /* 0x500 */ GsTexPacket texPacket; /* GsTexLoadPacket */
    /* 0x640 */ u8 clut[0x40];       /* from txtFontAlphaClut */
} TxtFontImage; /* size 0x680 */
#endif

/* ------------------------------------------------------------------------ */
/* sysState (0x004F0D00, 0x1400 bytes)                                       */
/* ------------------------------------------------------------------------ */
typedef struct SysState {
    /* 0x00 */ s32 frame;           /* main loop frame counter */
    /* 0x04 */ s32 vblankCount;     /* GsGetVblankCount() each frame */
    /* 0x08 */ u8 frameSkip;        /* GRX.INI "frame skip" (s8 in 5D2848) */
    /* 0x09 */ u8 frameMode;        /* GRX.INI "frame mode", vblanks per frame (GsVsyncFlip) */
    /* 0x0A */ u8 battleBoot;       /* sysBattleBoot: boot program was BATTLE */
    /* 0x0B */ u8 program;          /* current program: 0 VIEWER, 1 TITLE, 2 FIELD, 3 BATTLE, 4 ENDING, 5 MCCHECK, 6 AUTOMAP; 0xFF none */
    /* 0x0C */ s8 progCallback;     /* callback index SysProgCall is running */
    /* 0x0D */ u8 unkD;             /* set with a movie request (scenario 7/11 arg, 7/13 = 10, demo = 1) */
    /* 0x0E */ u8 unkE;             /* cleared with a movie request, set once it plays */
    /* 0x0F */ u8 progChangeReq;    /* sysProgChangeReq: program/movie change requested */
    /* 0x10 */ u8 movieNo;          /* SysMoviePlay(movieNo) */
    /* 0x11 */ u8 unk11;            /* (s8 in 5D2848) */
    /* 0x12 */ u8 demoBoot;         /* booted into the attract demo (program 1 at boot) */
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u16 nextMapNo;       /* map to start on after the change (demo: sysDemoMapIds; s16 in 5FD000) */
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u32 loopFrameCount;  /* sysLoopFrameCount; u32 decided by DbgLoadMeterDraw's sltiu (s32 in 4000C0, 403CC8, 404528) */
    /* 0x1C */ s32 debugMode;       /* sysDebugMode, GRX.INI "debug mode" */
    /* 0x20 */ s32 heap20;          /* heap handles (MemHeapCreate*) */
    /* 0x24 */ s32 battleHeap;      /* sysBattleHeap */
    /* 0x28 */ s32 battleMagicHeap; /* sysHeapBattleMagic */
    /* 0x2C */ s32 dataHeap;        /* sysDataHeap, "<<DATA PARTATION>>" */
    /* 0x30 */ s32 heap30;          /* font images are allocated here */
    /* 0x34 */ s32 faceHeap;        /* sysFaceHeap, "<<FACE PARTATION>>" */
    /* 0x38 */ s32 heap38;
    /* 0x3C */ s32 pauseMode;       /* 0 run, 1 paused, 2 single step (START with debug) */
    /* 0x40 */ s32 dbgMenuFlags;    /* sysDbgMenuFlags (u32 in 689A10, 7006E8) */
    /* 0x44 */ s32 dbgPage;         /* debug page (named mode/sel in a few views) */
    /* 0x48 */ s32 dbgCursor;       /* debug page line (u32 in 68DCE8) */
    /* 0x4C */ s32 dbgDigit;        /* digit/column edited by DbgEditValue and the track editors */
    /* 0x50 */ void *unk50;
    /* 0x54 */ f32 frameTime;       /* GsGetFrameTimePercent() at the end of the frame */
    /* 0x58 */ f32 frameTimeDelta;  /* sysPerfTimes[8] - sysPerfTimes[7] */
    /* 0x5C */ s32 screenCaptureAddr; /* sysScreenCaptureAddr */
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 screenCaptureReq; /* sysScreenCaptureReq (s8 in 420808 and the alias) */
    /* 0x62 */ u8 unk62;            /* (s8 in 420808) */
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65;
    /* 0x66 */ u16 unk66;
    /* 0x68 */ u8 activeCount;      /* active program count */
    /* 0x69 */ u8 active[8];        /* active programs, 0xFF empty */
    /* 0x71 */ u8 requested[8];     /* requested programs (s8 in 400CB8) */
    /* 0x79 */ u8 pad79[0x80 - 0x79];
    /* 0x80 */ TxtFontImage fontImages[3]; /* sysFontImage, sysFontImage2, menuHelpGlyphImage */
} SysState; /* size 0x1400 */

extern SysState sysState;

/* The window textures: the texture packets of the font images, as a
 * 0x680-stride array starting at sysState.fontImages[0].texPacket
 * (TxtWinTextureBind 0x005E9F20 binds 0-2 from here). */
typedef struct TxtWinTex {
    /* 0x000 */ GsTexPacket tex;
    /* 0x140 */ u8 pad140[0x680 - 0x140];
} TxtWinTex; /* size 0x680 */

extern TxtWinTex txtWinTextures[3];     /* 0x004F1280 = &sysState.fontImages[0].texPacket */

/* Aliases into sysState. */
extern u8 sysBattleBoot;         /* sysState.battleBoot */
extern u8 sysProgChangeReq;      /* sysState.progChangeReq */
extern u32 sysLoopFrameCount;    /* sysState.loopFrameCount */
extern s32 sysDebugMode;         /* sysState.debugMode */
extern s32 sysBattleHeap;        /* sysState.battleHeap */
extern s32 sysHeapBattleMagic[]; /* sysState.battleMagicHeap (declared as an array by MgcForceEnd) */
extern s32 sysDataHeap;          /* sysState.dataHeap */
extern s32 sysFaceHeap;          /* sysState.faceHeap */
extern s32 sysDbgMenuFlags;      /* sysState.dbgMenuFlags */
extern s32 sysScreenCaptureAddr; /* sysState.screenCaptureAddr */
extern s8 sysScreenCaptureReq;   /* sysState.screenCaptureReq */
extern TxtFontImage sysFontImage[];       /* sysState.fontImages[0] */
extern TxtFontImage sysFontImage2[];      /* sysState.fontImages[1] */
extern TxtFontImage menuHelpGlyphImage[]; /* sysState.fontImages[2] */

/* ------------------------------------------------------------------------ */
/* GRX.INI settings (sysIni 0x004F2100, 0x2E0), filled by SysIniLoad from the */
/* keys in sysIniKeyTable. The keys "program no", "debug mode", "frame mode"  */
/* and "frame skip" go to sysState instead.                                   */
/* ------------------------------------------------------------------------ */
/* Initial character record of the debug start, passed to PtyCharInit
 * (0x00410840) by DbgBattleSetup; PtyCharInit reads the arrays as u16. */
typedef struct SysIniParty {
    /* 0x00 */ s32 exp;       /* "partyN_exp" */
    /* 0x04 */ u16 equip[6];  /* "partyN_eq0".."eq5" */
    /* 0x10 */ u16 slot[6];   /* "partyN_sl0".."sl5" */
    /* 0x1C */ u16 skills[15];
    /* 0x3A */ u8 pad3A[2];
} SysIniParty; /* size 0x3C */

typedef struct SysIni {
    /* 0x000 */ u8 backColor[3];   /* "backcol r/g/b" (col[6]/col[10] arrays in some views) */
    /* 0x003 */ u8 fogColor[3];    /* "fogcolor r/g/b" */
    /* 0x006 */ u8 viewerNoLight;  /* "viewer lgt" (inverted: 'y' stores 0); s8 in SysIniLoad */
    /* 0x007 */ u8 viewerCrs;      /* "viewer crs" */
    /* 0x008 */ u8 viewerSdw;      /* "viewer sdw" */
    /* 0x009 */ u8 viewerStg;      /* "viewer stg" */
    /* 0x00A */ u8 viewerNoLoop;   /* "viewer lop" (inverted) */
    /* 0x00B */ u8 viewerPtc;      /* "viewer ptc" */
    /* 0x00C */ u8 ground0Dsp;     /* "ground0dsp" */
    /* 0x00D */ u8 useMdtData;     /* "usemdtdata" */
    /* 0x00E */ u8 viewerMip;      /* "viewer mip" (sysIniViewerMip) */
    /* 0x00F */ u8 encountOn;      /* "encount_on" */
    /* 0x010 */ u8 startScnr;      /* "start_scnr" */
    /* 0x011 */ u8 pad11[3];
    /* 0x014 */ s32 mainFlagNo;    /* "mainflagno" */
    /* 0x018 */ f32 fogDist;       /* "fog   dist" */
    /* 0x01C */ f32 clipFar;       /* "clipping f" */
    /* 0x020 */ f32 clipNear;      /* "clipping n" */
    /* 0x024 */ f32 viewAngle;     /* "view angle" */
    /* 0x028 */ f32 motionSpeed;   /* "motion spd" */
    /* 0x02C */ f32 motionSmooth;  /* "motion smt" */
    /* 0x030 */ f32 walkSpeed0;    /* "walkspeed0" */
    /* 0x034 */ s32 collision0;    /* "collision0" (0xFF / 0) */
    /* 0x038 */ s32 cameraAi0;     /* "camera_ai0" */
    /* 0x03C */ s32 mapNumber;     /* "map_number" (hex) */
    /* 0x040 */ s32 mapWarpNo;     /* "map_warpno" (hex) */
    /* 0x044 */ u8 pad44[0x50 - 0x44];
    /* 0x050 */ f32 ambient[4];    /* "ambient r/g/b/a" */
    /* 0x060 */ f32 light0Color[4]; /* "light0 r/g/b/a" */
    /* 0x070 */ f32 light0Angle[4]; /* "light0 x/y" ([2] used; DbgLightAngleSet) */
    /* 0x080 */ f32 light1Color[4]; /* "light1 r/g/b/a" */
    /* 0x090 */ f32 light1Angle[4]; /* "light1 x/y" */
    /* 0x0A0 */ SysIniParty party[4];
    /* 0x190 */ u8 pad190[0x280 - 0x190];
    /* 0x280 */ u8 partyExt[8];    /* "party_ext0".."7"; u8 decided by DbgBattleSetup's lbu (s8 in SysIniLoad) */
    /* 0x288 */ u16 partyNum[4];   /* "partyN_num": 0x1000 + character; u16 decided by DbgBattleSetup's lhu (s16 in SysIniLoad) */
    /* 0x290 */ u8 pad290[0x298 - 0x290];
    /* 0x298 */ s32 monsNo[4];     /* "mons_0N_no" (hex) */
    /* 0x2A8 */ s32 monsNum[4];    /* "mons_0Nnum" (u32 in 41FE40) */
    /* 0x2B8 */ s32 monsLv[4];     /* "mons_0N_lv" */
    /* 0x2C8 */ s32 mapSeBank[4];  /* "mapse_bnk0".."3" */
    /* 0x2D8 */ u8 pad2D8[8];
} SysIni; /* size 0x2E0 */

extern SysIni sysIni;
extern u8 sysIniViewerMip;      /* sysIni.viewerMip */
extern char sysIniKeyTable[150][10];
extern char sysIniFileName[];

/* ------------------------------------------------------------------------ */
/* Program table (sysProgramTable 0x00499B00, 7 x 0x34)                      */
/* ------------------------------------------------------------------------ */
typedef s32 (*SysProgFunc)(s32, s32);

typedef struct SysProgram {
    /* 0x00 */ char fileName[0x10]; /* "VIEWER.BIN" ... read by SysProgLoad */
    /* 0x10 */ SysProgFunc entry;   /* load address = callback 0 (s32 in 400988) */
    /* 0x14 */ SysProgFunc unk14;   /* callback 1 */
    /* 0x18 */ SysProgFunc unk18;   /* callback 2, called when the program becomes active */
    /* 0x1C */ SysProgFunc unk1C;   /* callback 3 */
    /* 0x20 */ SysProgFunc update;  /* callback 5 (SysProgUpdateAll) */
    /* 0x24 */ SysProgFunc buildDraw; /* callback 8 (SysProgBuildDrawAll) */
    /* 0x28 */ SysProgFunc draw;    /* callback 4 (SysProgDrawAll) */
    /* 0x2C */ SysProgFunc endFrame; /* callback 6 (SysProgEndFrameAll) */
    /* 0x30 */ SysProgFunc debugMenu; /* callback 7 (SysProgDebugMenu) */
} SysProgram; /* size 0x34; SysProgRegister stores s32 arguments into the callbacks */

extern SysProgram sysProgramTable[7];
extern char sysProgramNames[][16];

/* ------------------------------------------------------------------------ */
/* Attract demo (sysDemo 0x004AC300, 0x440)                                  */
/* ------------------------------------------------------------------------ */
typedef struct SysDemo {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 mode;            /* sysDemoMode, SysDemoGetMode (state in 41F0F8) */
    /* 0x002 */ u8 ready;           /* sysDemoReady, SysDemoSetReady */
    /* 0x003 */ u8 blinkTimer;      /* prompt blink, wraps at 120 */
    /* 0x004 */ u8 pad4[0x40 - 0x4];
    /* 0x040 */ u32 timer;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ u8 pad48[0x4C - 0x48];
    /* 0x04C */ u32 idleCount;      /* frames without input on the title */
    /* 0x050 */ u8 unk50;
    /* 0x051 */ u8 unk51;
    /* 0x052 */ u8 step;
    /* 0x053 */ u8 unk53[4];
    /* 0x057 */ u8 skipped;         /* sysDemoSkipped: START ended the demo */
    /* 0x058 */ u16 unk58;
    /* 0x05A */ u8 pad5A[2];
    /* 0x05C */ f32 promptAlpha;    /* SysDemoPromptPrims; s32 in 41F0F8 (never read there), stored as s32 0 by SysDemoStart */
    /* 0x060 */ f32 captionAlpha0;  /* SysDemoCaptionPrims */
    /* 0x064 */ f32 captionAlpha1;
    /* 0x068 */ u8 pad68[0x70 - 0x68];
    /* 0x070 */ GphFade fade;
    /* 0x1A0 */ GphFade fade2;
    /* 0x2D0 */ u8 pad2D0[0x300 - 0x2D0];
    /* 0x300 */ u8 texPacket[0x140]; /* GsTexLoadPacket of GRXSYS +0x150 */
} SysDemo; /* size 0x440 */

extern SysDemo sysDemo;
extern u8 sysDemoMode;          /* sysDemo.mode */
extern u8 sysDemoReady;         /* sysDemo.ready */
extern u8 sysDemoSkipped;       /* sysDemo.skipped */
extern u16 sysDemoMapIds[6];

/* ------------------------------------------------------------------------ */
/* 2D sprite state (sprState 0x004F0A80; sprSavedState 0x004ABCA8)           */
/* ------------------------------------------------------------------------ */
typedef struct SprColor {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
} SprColor;

typedef struct SprState {
    /* 0x00 */ f32 sx;          /* scale */
    /* 0x04 */ f32 sy;
    /* 0x08 */ s32 angle;       /* 16-bit angle units */
    /* 0x0C */ s32 flags;       /* 1 flip U, 2 flip V (u32 in 6A2F08 ...) */
    /* 0x10 */ SprColor col[4]; /* vertex colours; most code sets col[0] only (plain r/g/b/a fields in many views, a bitfield in 64DC60) */
    /* 0x20 */ s32 blend;       /* 1 after SprBegin, 2 = additive in the battle HUD */
    /* 0x24 */ f32 ox;          /* origin */
    /* 0x28 */ f32 oy;
    /* 0x2C */ s16 offX;        /* offset added to x/y before scaling */
    /* 0x2E */ s16 offY;
    /* 0x30 */ s8 skewX;
    /* 0x31 */ s8 skewY;
} SprState; /* size 0x34 */

extern SprState sprState;
extern SprState sprSavedState;  /* SprPushTransform / SprPopTransform */
extern s8 sprQuadCount;
extern s8 sprRectCount;
extern s8 sprQuad3DCount;
extern f32 sprFocalLength;

/* Texture rectangle of a sprite (SprDrawRect, SprDrawRectFast): size, then
 * the UV corners (u16: SprDrawRectFast 40F768 and SprDrawRect 40F0B0 compare
 * them unsigned). Also the menu icon tables, the digit font glyphs and the
 * minigame layout rects. */
typedef struct SprRect {
    /* 0x0 */ s16 w;
    /* 0x2 */ s16 h;
    /* 0x4 */ u16 u0;
    /* 0x6 */ u16 v0;
    /* 0x8 */ u16 u1;
    /* 0xA */ u16 v1;
} SprRect; /* size 0xC */

/* Icon tables (main data, shared with FIELD and BATTLE). */
extern SprRect menuHelpVsTypeIcons[];   /* 0x004A5B90, by menuHelpVsType */
extern const SprRect menuSkillTypeIcons[]; /* 0x004A5F78, 8 by skill type (ptySkillTable +2) */
extern const SprRect menuStatusIcons[]; /* 0x004A6418, status icons */

/* An icon table entry: SprRect read with unsigned sizes. The FIELD menus
 * read the tables below as these (their width/height reads only match
 * unsigned); SprDrawRect callers pass an entry as a SprRect *. */
typedef struct MenuIconRect {
    /* 0x0 */ u16 w;
    /* 0x2 */ u16 h;
    /* 0x4 */ u16 u0;
    /* 0x6 */ u16 v0;
    /* 0x8 */ u16 u1;
    /* 0xA */ u16 v1;
} MenuIconRect; /* size 0xC */

extern const MenuIconRect menuElementIcons[]; /* 0x004A5E70, by element number */
extern const MenuIconRect fldMenuJunctionIcons[]; /* 0x004A5FB8, 7 junction fly-in pieces (also battle unit flag bits 0-6) */
extern const MenuIconRect menuItemIcons[];   /* 0x004A6010, 80 item icons by ItemGetIcon ([78] menuIconEggSlotEmpty) */
extern const SprRect sprStarIconRects[]; /* 0x004A5F60, [0] a small star (ChrStunStarsDraw, menus), [1] */

/* Camera-facing matrix built by 0x0044A058 (BATTLE: 6ADF98) for SprDrawRect3D. */
extern FMATRIX sprBillboardMatrix;      /* 0x004ED740 */

/* SprDrawRectList / SprDrawRectFastList entry; a NULL rect ends the list. */
typedef struct SprRectListEntry {
    /* 0x0 */ SprRect *rect;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
} SprRectListEntry; /* size 0x8 */

/* Digit font of a number field (MenuNumberLayout 0x00417E80): rects[0..9]
 * the digits, then extra marks (w[10]: the overflow mark of a menu number
 * field; menuDigitFont 13 slash, fldMenuDigitFont2 11 percent suffix). */
typedef struct SprDigitFont {
    /* 0x0 */ SprRect *rects;
    /* 0x4 */ s8 w[12];         /* advance per glyph */
} SprDigitFont; /* size 0x10 */

/* A 0xRRGGBBAA vertex colour word. SprDrawColorQuad reads its bytes (b[0] is
 * alpha, so c.r is really alpha too: keep each file's member), the tinted
 * variant and BATTLE save and restore whole words. */
typedef union SprCol {
    u32 word;
    s32 w;
    u8 b[4];
    struct {
        u8 r, g, b, a;
    } c;
} SprCol; /* size 0x4 */

/* Colour quad (SprDrawColorQuad 0x0040F9E8): size, origin and four corner
 * colours. */
typedef struct SprColorQuad {
    /* 0x00 */ s16 w;
    /* 0x02 */ s16 h;
    /* 0x04 */ s16 originX;     /* subtracted from the position (SprState offX/offY) */
    /* 0x06 */ s16 originY;
    /* 0x08 */ SprCol col[4];   /* 0x093E334C / 0x7F574C by default (MenuFrameInit) */
} SprColorQuad; /* size 0x18 */

/* Window frame record (MenuFrameInit 0x00418310, MenuFrameDraw 0x00418390);
 * BATTLE's frames are the same record. */
typedef struct MenuFrame {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 sx;          /* scale */
    /* 0x0C */ f32 sy;
    /* 0x10 */ s16 w;
    /* 0x12 */ s16 h;
    /* 0x14 */ s16 ox;          /* origin (sprState.offX = -ox); w / 2 in the help window.
                                 * Read as u16 in 4 places in 41A108/419960 ((u16) casts) */
    /* 0x16 */ s16 oy;
    /* 0x18 */ s32 angle;
    /* 0x1C */ f32 alpha;       /* nothing drawn when <= 0 */
    /* 0x20 */ s32 unk20;
    /* 0x24 */ SprColorQuad bg; /* background, w + 2 by h + 2 */
} MenuFrame; /* size 0x3C */

/* Number field (MenuNumberLayout 0x00417E80, MenuNumberDraw 0x00417E40). */
typedef struct MenuNumField {
    /* 0x00 */ SprDigitFont *font;
    /* 0x04 */ SprRectListEntry *sprites; /* output list (a stack buffer in MenuNumberDraw) */
    /* 0x08 */ f32 sx;          /* scale, the number is right-aligned at x */
    /* 0x0C */ f32 sy;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s8 digits;       /* field size; a bigger number shows digits overflow marks */
    /* 0x15 */ u8 pad15[3];
} MenuNumField; /* size 0x18 */

/* ------------------------------------------------------------------------ */
/* Bitmap font text (Txt* 0x00416D40-0x004172E0, MenuTextDraw 0x004174C0)    */
/* ------------------------------------------------------------------------ */
/* Glyph buffer: a font image of 18x18 cells that strings are rendered into
 * cell by cell (TxtGlyphBufInit/Reset/Put, TxtStringWidth). 0x14 bytes
 * (btlCmdListGlyphBufs[2] steps 0x14). */
typedef struct TxtGlyphBuf {
    /* 0x00 */ struct TxtFontImage *img; /* s32 in TxtGlyphBufPut (passed on as s32) */
    /* 0x04 */ s16 cur;         /* next cell; u16 in the TxtObj setters */
    /* 0x06 */ s16 count;       /* cells in the image */
    /* 0x08 */ s16 first;       /* first cell used */
    /* 0x0A */ s16 last;        /* last cell used; full past it */
    /* 0x0C */ s8 cols;         /* cells per row */
    /* 0x0D */ s8 col;          /* column of cur (u8 in TxtGlyphBufReset) */
    /* 0x0E */ s8 row;          /* row of cur (u8 in TxtGlyphBufReset) */
    /* 0x0F */ u8 cellW;        /* 18 */
    /* 0x10 */ u8 cellH;        /* 18 */
    /* 0x11 */ u8 pad11[3];
} TxtGlyphBuf; /* size 0x14 */

/* TxtObj.style */
#define TXT_STYLE_SHADOW   0x1  /* low 4 bits: 1 drop shadow, 2 outline */
#define TXT_STYLE_OUTLINE  0x2
#define TXT_STYLE_GRADIENT 0x10 /* vertical colour gradient (menuTextGrad*) */
#define TXT_STYLE_SKEW     0x20 /* skewed by menuTextOffsetX/Y */

/* Text object (0x24): a string laid out from a glyph buffer by
 * TxtObjSetString (0x004171C0) / TxtObjSetStringList (0x00417250) and drawn
 * by MenuTextDraw (0x004174C0). */
typedef struct TxtObj {
    /* 0x00 */ TxtGlyphBuf *glyphs; /* glyph buffer the string was put into */
    /* 0x04 */ SprRectListEntry *sprites; /* allocated by MenuTextDraw for one draw */
    /* 0x08 */ SprRect *rects;
    /* 0x0C */ f32 alignX;          /* x -= width * alignX (MenuTextLayout) */
    /* 0x10 */ f32 alignY;          /* y -= 24 * alignY */
    /* 0x14 */ s16 x;               /* position (help window: relative to the frame origin) */
    /* 0x16 */ s16 y;
    /* 0x18 */ f32 scale;
    /* 0x1C */ u8 style;            /* TXT_STYLE_*; s8 in the help window views (stores only) */
    /* 0x1D */ s8 spacing;          /* extra pixels per glyph (TxtStringWidth, MenuTextLayout) */
    /* 0x1E */ s16 width;           /* string width * scale (TxtObjSetString) */
    /* 0x20 */ s16 glyphFirst;      /* first glyph buffer cell (u16 in 4171C0, 417358) */
    /* 0x22 */ s16 glyphLast;       /* last cell; nothing drawn when < glyphFirst */
} TxtObj; /* size 0x24 */

/* ------------------------------------------------------------------------ */
/* Field <-> battle hand-off block (btlHandoff 0x004F23E0, 0x140), filled by  */
/* FldBattleStart / DbgBattleSetup, read by BATTLE, read back by              */
/* FldBattleReturn. Units 0-3 are the party, 4-15 the enemies. The per-kind  */
/* arrays at +0xC4.. are split into party and enemy halves as FldBattleStart */
/* and DbgBattleSetup write them (6BC620 and 5F1E78 index the               */
/* two halves as one [8] array).                                             */
/* ------------------------------------------------------------------------ */
struct ChrPackBinding; /* character pack binding filled by ChrPackBind (defined elsewhere) */

typedef struct BtlHandoff {
    /* 0x000 */ u16 unk0;          /* main loop leaves the program when this or softResetReq is set (s16 in 5D2848) */
    /* 0x002 */ u16 softResetLock; /* sysSoftResetLock; u16 decided by SysMain's lhu (s16 in 5D2848, 636BE0 and the alias) */
    /* 0x004 */ s32 softResetReq;  /* sysSoftResetReq: return-to-title request */
    /* 0x008 */ s32 result;        /* btlHandoffResult: 2 escaped, 3 defeated */
    /* 0x00C */ u16 unkC;          /* set to 1 by 61BBC8 (spot item), cleared by 5D50C0; 5D4500 reads it with lhu */
    /* 0x00E */ u16 dropCount;     /* btlHandoffDropCount (s16 there and in 6800E0) */
    /* 0x010 */ f32 pos[4] __attribute__((aligned(16))); /* player position, copied with lq/sq */
    /* 0x020 */ s32 dropIds[12];   /* item ids; 5D4500 reads the low halfword (lhu) */
    /* 0x050 */ s32 unk50;         /* encounter tenant index (ChrTenantGet, EvtMapFlagSet) */
    /* 0x054 */ s32 unk54;
    /* 0x058 */ u8 pad58[4];
    /* 0x05C */ u8 ipStopOpt;      /* btlHandoffIpStopOpt = save option +0x28 */
    /* 0x05D */ u8 debugStart;
    /* 0x05E */ s16 startType;     /* fldBtlStartTypeNormal/Ambush + bonus */
    /* 0x060 */ s16 unk60;
    /* 0x062 */ s16 unk62;
    /* 0x064 */ s16 unk64;
    /* 0x066 */ s16 unk66;
    /* 0x068 */ s16 unk68;
    /* 0x06A */ s16 unk6A;
    /* 0x06C */ s16 unk6C;
    /* 0x06E */ u8 pad6E;
    /* 0x06F */ u8 endFade;        /* btlHandoffEndFade: 1 black, 2 white (s8 in 6191B8) */
    /* 0x070 */ s8 partyCount;     /* ChrPartyCountForBattle (u8 in 41FE40) */
    /* 0x071 */ s8 unk71;
    /* 0x072 */ s8 unk72;
    /* 0x073 */ u8 pad73[0x78 - 0x73];
    /* 0x078 */ s32 encType;       /* 1 Forestall, 2 Surprised (u32 in 41FE40) */
    /* 0x07C */ u8 pad7C[4];
    /* 0x080 */ s8 unitFlags[16];  /* (u8 in 680DB0, 41FE40) */
    /* 0x090 */ u16 unitIds[16];   /* 0x1000 + character for the party (s16 in 6D9950, 5F1E78) */
    /* 0x0B0 */ u16 mapNo;         /* btlEntryMapNo, battle stage / EnvParamGet key (s16 in 619C80, 41FE40) */
    /* 0x0B2 */ u8 padB2[2];
    /* 0x0B4 */ s32 unkB4;         /* sky info */
    /* 0x0B8 */ u8 padB8[0xC4 - 0xB8];
    /* 0x0C4 */ s32 partyLoaded[4];
    /* 0x0D4 */ s32 enemyLoaded[4];
    /* 0x0E4 */ s32 partyKind[4];     /* 0x1000 + character; s32 decided by FldBattleStart's store order */
    /* 0x0F4 */ u32 enemyKind[4];     /* enemy id; u32 decided by FldBattleReturn's switch (s32 in the other views) */
    /* 0x104 */ struct ChrPackBinding *partyModels[4]; /* fldCharModelDefs / dbgPartyModels entries; any non-void pointer (FldBattleStart's store order) */
    /* 0x114 */ struct ChrPackBinding *enemyModels[4]; /* fldChrPackTable / dbgEnemyModels entries (void * in 619C80) */
    /* 0x124 */ s32 stagePack;     /* mapModelPack / dbgStagePack (void * in 41FE40) */
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
    /* 0x130 */ u8 pad130[0x10];
} BtlHandoff; /* size 0x140 */

extern BtlHandoff btlHandoff;
extern u16 sysSoftResetLock;     /* btlHandoff.softResetLock (only stored through the alias) */
extern s32 sysSoftResetReq;      /* btlHandoff.softResetReq */
extern s32 btlHandoffResult;     /* btlHandoff.result */
extern s16 btlHandoffDropCount;  /* btlHandoff.dropCount */
extern u8 btlHandoffIpStopOpt;   /* btlHandoff.ipStopOpt */
extern u8 btlHandoffEndFade;     /* btlHandoff.endFade */
extern u16 btlEntryMapNo;        /* btlHandoff.mapNo */

/* ------------------------------------------------------------------------ */
/* Other system state                                                        */
/* ------------------------------------------------------------------------ */
typedef struct SysPartyPac {
    /* 0x00 */ s32 packs[10];   /* relocated character packs (void * in 41FE40) */
    /* 0x28 */ s32 *offsets;    /* SYS/PARTY.PAC offset table */
} SysPartyPac; /* size 0x2C */

extern SysPartyPac sysPartyPac;
extern f32 sysPerfTimes[11];    /* GsGetFrameTimePercent samples of the frame loop (PERFORMANCE page) */
extern s32 sysIsNtsc;
extern s32 sysFrameCount;
extern u8 sysNewGameFlag;

#endif
