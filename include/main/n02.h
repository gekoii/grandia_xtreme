#ifndef MAIN_N02_H
#define MAIN_N02_H

/*
 * Main executable 0x004172E0-0x0042C330: menu widgets shared by FIELD and
 * BATTLE (text colour, hand cursor, sparkles, left/right arrows, window
 * frame), the help window (menuHelp), system data loading, the attract demo
 * prompts and the debug pages. Globals here are used only by these files;
 * the shared ones are in sys.h, pad.h, gph.h, camera.h, party.h and grxsys.h.
 */

#include "common.h"
#include "sys.h"   /* SprRect, MenuFrame, MenuNumField, TxtObj */

/* ------------------------------------------------------------------------ */
/* Menu widgets (0x004172E0-0x00418F88)                                      */
/* ------------------------------------------------------------------------ */

/* Colour/offset for the next MenuTextDraw (reset after each draw). */
extern u8 menuTextAddR;         /* added colour (s8 in 417DE8, stores only) */
extern u8 menuTextAddG;
extern u8 menuTextAddB;
extern u8 menuTextGradR;        /* bottom colour with TXT_STYLE_GRADIENT */
extern u8 menuTextGradGValue;
extern u8 menuTextGradB;
extern u8 menuTextOffsetX;      /* skew with TXT_STYLE_SKEW (s8 in 417DE8, stores only) */
extern u8 menuTextOffsetY;

/* Hand cursor (menuCursor 0x004ABCE8). */
typedef struct MenuCursor {
    /* 0x00 */ s8 mode;         /* 0 hidden, 1 resting (bobs), 2 moving */
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u16 timer;       /* frames in the mode (bob phase); s16 in 418730/418770 */
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 targetX;
    /* 0x10 */ f32 targetY;
    /* 0x14 */ f32 velX;
    /* 0x18 */ f32 velY;
    /* 0x1C */ u8 pad1C[4];
} MenuCursor; /* size 0x20 */

/* Left/right arrow pair (menuLrArrows 0x004ABD08). */
typedef struct MenuLrArrows {
    /* 0x0 */ s16 x;            /* centre */
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 halfWidth;    /* arrows drawn at x -/+ halfWidth */
    /* 0x6 */ s8 flashSide;     /* 1 left, 2 right, 0 none (MenuLrArrowsFlash) */
    /* 0x7 */ u8 flashTimer;    /* 0x20 frames; s8 in 418DA0 (store only) */
} MenuLrArrows; /* size 0x8 */

/* Sparkle particle (menuSparkles 0x004ABD10, 32 of them). */
typedef struct MenuSparkle {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 speed;        /* added to y each frame, +0.05 per frame */
    /* 0xC */ u8 timer;         /* 0xA0 at spawn, -2 per frame; alpha */
    /* 0xD */ u8 padD;
    /* 0xE */ u16 angle;        /* random rotation (s16 in 418B10, store only) */
} MenuSparkle; /* size 0x10 */

/* MenuFrame and MenuNumField (shared with BATTLE) are in sys.h. */

extern s32 menuWidgetFrame;     /* frame counter of the widgets (blinking, bobbing) */
extern MenuCursor menuCursor;
extern MenuLrArrows menuLrArrows;
extern MenuSparkle menuSparkles[32];
extern s8 menuSparkleLast;      /* last spawned sparkle, -1 = none */

/* ------------------------------------------------------------------------ */
/* Help window (menuHelp 0x004ABF18), 0x00418F88-0x0041CFF8                  */
/* ------------------------------------------------------------------------ */

/* MenuHelp.lines[] content kinds */
#define MENU_HELP_LINE_NONE    0
#define MENU_HELP_LINE_NAME    1
#define MENU_HELP_LINE_DESC    2
#define MENU_HELP_LINE_STATS   4  /* menuHelpStatLines */
#define MENU_HELP_LINE_ICONS   5  /* effect label / weapon icons (menuHelpWeaponIcons) */
#define MENU_HELP_LINE_ELEMENT 6
#define MENU_HELP_LINE_STATUS  7  /* status effect / immunity */
#define MENU_HELP_LINE_VS      8  /* strong vs / reduces damage (text[16]) */
#define MENU_HELP_LINE_LINK    9  /* linked skill (menuHelpLinkNumField) */

typedef struct MenuHelp {
    /* 0x000 */ u8 state;       /* 0 closed, 0x10 open requested, 0x20 opening, 0x21 open, 0x30 closing */
    /* 0x001 */ s8 slideTimer;  /* 8-frame slide from (fromX, fromY) */
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ s32 timer;
    /* 0x008 */ s32 openFrame;  /* menuHelpFrame at the request */
    /* 0x00C */ f32 alpha;
    /* 0x010 */ s8 style;       /* 0 popup, 1 bottom bar, 2 tactics */
    /* 0x011 */ s8 nextStyle;   /* 0x7F none */
    /* 0x012 */ s8 kind;        /* data: 0/1 PtyMove (skill/magic), 2 PtySkillDef, 3 item, 4 weapon,
                                 * 5 armor (PtyItem); with style 2 the tactics index */
    /* 0x013 */ s8 nextKind;
    /* 0x014 */ void *data;     /* record shown, by kind (party.h types); s32 in 419098/41B2F8 */
    /* 0x018 */ void *nextData;
    /* 0x01C */ f32 fromX;
    /* 0x020 */ f32 fromY;
    /* 0x024 */ f32 toX;
    /* 0x028 */ f32 toY;
    /* 0x02C */ s16 w;
    /* 0x02E */ s16 h;
    /* 0x030 */ s8 lines[4];    /* content of each line, MENU_HELP_LINE_* */
    /* 0x034 */ TxtObj text[17]; /* [16] is the "strong vs" / "reduces damage" label */
    /* 0x298 */ MenuFrame frame;
    /* 0x2D4 */ u8 pad2D4[4];
} MenuHelp; /* size 0x2D8 */

/* Stat line (menuHelpStatLines 0x004AC208, 4 of them). */
typedef struct MenuStatLine {
    /* 0x00 */ TxtObj text;
    /* 0x24 */ s16 value;       /* u16 in 41A108 (store only) */
    /* 0x26 */ s8 kind;         /* menuHelpStatNames index, -1 unused */
    /* 0x27 */ u8 pad27;
} MenuStatLine; /* size 0x28 */

extern MenuHelp menuHelp;
extern u8 menuHelpGlyphBuf[];   /* glyph buffer of the help window text (over menuHelpGlyphImage) */
extern MenuStatLine menuHelpStatLines[4];
extern s32 menuHelpFrame;
extern MenuNumField menuHelpLinkNumField; /* linked skill level number */

/* ------------------------------------------------------------------------ */
/* System data (0x0041CFF8-0x0041E228)                                       */
/* ------------------------------------------------------------------------ */
extern s8 sysCommonModelGroups[100]; /* model group per common model id, -1 = 0 (SysCommonModelGet) */

/* ------------------------------------------------------------------------ */
/* Debug pages (0x00420720-0x0042C330)                                       */
/* ------------------------------------------------------------------------ */
struct Camera;   /* camera.h */
struct GphModel; /* gph.h */

/* Model list shown by the OBJECT/MOTION/ATR pages (DbgSetObjList). */
typedef struct DbgObjList {
    /* 0x0 */ u32 count;
    /* 0x4 */ struct GphModel *objs[1]; /* count models */
} DbgObjList;

/* Light direction edited on the ENVIROMENTS page, degrees (+-360). */
typedef struct DbgLightAngle {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} DbgLightAngle; /* size 0x10 */

typedef struct DbgColor {
    /* 0x0 */ f32 r;
    /* 0x4 */ f32 g;
    /* 0x8 */ f32 b;
    /* 0xC */ f32 a;
} DbgColor; /* size 0x10 */

/* MEMORY VIEW & EDIT page (DbgMemDumpPage). */
typedef struct DbgMemView {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 ofs;         /* view offset in 8-byte units from base */
    /* 0x08 */ s32 mode;        /* 0-4: 128/64/32/16/8-bit columns */
    /* 0x0C */ s32 edit;        /* edit mode on */
    /* 0x10 */ s32 col;         /* edit cursor: nibble column */
    /* 0x14 */ s32 row;         /* edit cursor: line */
    /* 0x18 */ s32 base;        /* dbgMemDumpBase, 0x770000 by default (DbgViewInit) */
} DbgMemView; /* size 0x1C */

extern s8 dbgObjNo;             /* object selected on the debug pages (DbgGetObjNo) */
extern u8 dbgClearR;            /* clear colour (DbgSetClearColor); s8 in 420778/420808 (stores only) */
extern u8 dbgClearG;
extern u8 dbgClearB;
extern DbgColor dbgEnvMapParam;     /* env-map colour; a = strength (GphEnvMapSetColor) */
extern DbgLightAngle dbgLightAngles[8];
extern struct Camera *dbgCamera;    /* CAMERA/ENVIROMENTS pages (DbgSetCamera) */
extern DbgObjList *dbgObjList;
extern DbgMemView dbgMemView;
extern s32 dbgMemDumpBase;          /* = dbgMemView.base, set from the MEMORY page */

/* Strings shared by several pages */
extern char dbgStrPageHiliteOn[];
extern char dbgStrPageHiliteOff[];
extern char dbgStrDispOn[];
extern char dbgStrDispOff[];
extern char dbgPathSep[];
extern char dbgPathGbxExt[];

#endif
