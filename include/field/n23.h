/*
 * FIELD collision statistics (0x0061D210-0x0061DAD8) and the field menu
 * screens (0x0061EE40-0x00634408): the menu buffers, the per-screen state
 * blocks (fldMenuPartyPanel, fldMenuItem, fldMenuOptions, ...), the menu
 * texture objects and digit fonts. fldMenuWork, the windows, the cursor
 * and itemWork are in field/menu.h.
 *
 * Layouts were merged from the per-file views by byte offset.
 */
#ifndef FIELD_N23_H
#define FIELD_N23_H

#include "common.h"
#include "sys.h"
#include "main/n02.h"

/* ---------------------------------------------------------------- collision statistics */

/* Counters of the collision gather/sweep passes (FldCollGather 0x0061D210,
 * FldCollSweep 0x0061D2E8 and the asm part tests). */
extern s32 fldCollTriTestCount;   /* triangles tested in FldCollPartGatherSphere */
extern s32 fldCollTriBoundCount;  /* ... passing VuSphereTriBoundHit */
extern s32 fldCollTriPlaneCount;  /* ... passing VuTriPlaneNear */
extern s32 fldCollTriHitCount;    /* ... passing VuPointInTriangle */
extern s32 fldCollStat10;         /* cleared by FldCollGather, otherwise unused */
extern s32 fldCollMeshHitCount;   /* meshes passing the bound test in FldCollPart* */
extern s32 fldCollTriTestTotal;   /* triangles tested in FldCollPart* */
extern s32 fldCollPartIndex;      /* part index in FldCollModelGather* */

/* ---------------------------------------------------------------- menu buffers */

/* FldMenuWorkInit allocates these three in a row (0x00678918..0x00678923). */
extern void *fldMenuRippleWork;   /* 128x128 ripple work */
extern void *fldMenuImageBuf;     /* 0x4000 */
extern void *fldMenuParticleBuf;  /* 0x780 */

/* ---------------------------------------------------------------- textures */

/* GsTexPacket (a GS texture object, 0x140 bytes) is in gph.h (via sys.h). */

/* The 9 menu textures loaded from GRXSYS by FldMenuWorkInit. */
extern GsTexPacket fldMenuTextures[9];
extern GsTexPacket fldMenuTexLabels;     /* fldMenuTextures[1] */
extern GsTexPacket fldMenuTexLabels2;    /* fldMenuTextures[2] */
extern GsTexPacket fldMenuTex3;          /* fldMenuTextures[3] */
extern GsTexPacket fldMenuTexIcons;      /* fldMenuTextures[4] */
extern GsTexPacket fldMenuTexFaces;      /* fldMenuTextures[6] */
extern GsTexPacket fldMenuTexPopup;      /* fldMenuTextures[8] */

/* ---------------------------------------------------------------- digit fonts */

/* Menu digit fonts (SprDigitFont, sys.h): glyphs 0-9 digits, then extra
 * marks (menuDigitFont 13 slash, fldMenuDigitFont2 11 percent suffix). */
extern const SprDigitFont menuDigitFont;     /* rects menuLargeDigitSprites */
extern const SprDigitFont fldMenuDigitFont2; /* rects 0x00675170 */

/* ---------------------------------------------------------------- party panels */

/* The party member panels of the top menu and screen 1 (cleared by
 * FldMenuTopInit, 0x2C bytes). */
typedef struct FldMenuPartyPanel {
    /* 0x00 */ s32 hp[4];          /* shown HP, counts toward ItemCharPage.hp */
    /* 0x10 */ s32 sp[4];          /* shown SP, counts toward ItemCharPage.sp */
    /* 0x20 */ u8 tick;            /* toggles every frame; counting on odd frames */
    /* 0x21 */ u8 flash[4];        /* popup flash per member (FldMenuPartyPanelFlash) */
    /* 0x25 */ u8 flashTimer[4];
    /* 0x29 */ u8 row;             /* screen 1 2x2 cursor */
    /* 0x2A */ u8 col;
    /* 0x2B */ u8 pad2B;
} FldMenuPartyPanel; /* size 0x2C */

extern FldMenuPartyPanel fldMenuPartyPanel;
/* menuStatusIcons (status icons, shared with BATTLE and the help window) is in sys.h. */
extern u8 fldMenuScreen1WinIds[4];  /* {4,5,2,3} window per 2x2 cell */

/* ---------------------------------------------------------------- item screen */

/* Item screen state (screen 2; cleared by FldMenuItemInit, 0xE8 bytes). */
typedef struct FldMenuItem {
    /* 0x00 */ s8 category;        /* list shown: 0 bag, 1 eggs, 2 books, 3 skills, 4 valuables, 5 */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u32 top;            /* first list row shown */
    /* 0x10 */ u32 cursor;         /* fldMenuItemCursor */
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ u8 skillGauge;      /* > 0x80: skill level pips instead of the exp fraction (FldMenuItemListLayout 627320) */
    /* 0x19 */ u8 pad19;
    /* 0x1A */ u8 pipLen[0x1D];    /* pip size per level (627320) */
    /* 0x37 */ s8 shrink;          /* icon shrink of the empty slots, category 5 (627320) */
    /* 0x38 */ s8 marks[0x80];     /* per-row slide offsets (627320 steps them by -8), cleared by FldMenuItemReset */
    /* 0xB8 */ u8 padB8[9];
    /* 0xC1 */ u8 unkC1;
    /* 0xC2 */ u8 padC2[2];
    /* 0xC4 */ s32 unkC4;          /* -1 after a reset */
    /* 0xC8 */ s32 helpType;       /* help window kind of the selected row (627320) */
    /* 0xCC */ s32 helpId;
    /* 0xD0 */ u16 ang;            /* star rotation (627320) */
    /* 0xD2 */ u8 padD2[2];
    /* 0xD4 */ u8 page;            /* text page of the list captions (627320) */
    /* 0xD5 */ u8 padD5[0x13];
} FldMenuItem; /* size 0xE8 */

extern FldMenuItem fldMenuItem;

/* ---------------------------------------------------------------- options screen */

/* Options screen state (screen 3; FldMenuOptionsInit, 0x1A bytes): the
 * option values being edited, copied from and back to sysSaveData. */
typedef struct FldMenuOptions {
    /* 0x00 */ s8 state;           /* 0 choosing an option, 1 changing it */
    /* 0x01 */ u8 cursor;          /* option 0-6 */
    /* 0x02 */ u8 value;           /* value being changed */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s8 rowX[7];         /* row slide-in offsets */
    /* 0x0B */ u8 padB[7];
    /* 0x12 */ u8 vibration;       /* sysSaveData.optVibration */
    /* 0x13 */ u8 camReverse;
    /* 0x14 */ u8 soundMode;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 ipStop;
    /* 0x17 */ u8 miniMapOff;
    /* 0x18 */ u8 miniMap2A;
    /* 0x19 */ u8 miniMap2B;
} FldMenuOptions; /* size 0x1A */

extern FldMenuOptions fldMenuOptions;

/* ---------------------------------------------------------------- party order screen */

/* Party order screen state (screen 4; FldMenuPartyOrderInit, 0x12 bytes). */
typedef struct FldMenuPartyOrder {
    /* 0x00 */ s8 state;
    /* 0x01 */ s8 sub;
    /* 0x02 */ u8 cursor;
    /* 0x03 */ u8 sel;
    /* 0x04 */ u8 yesno;
    /* 0x05 */ u8 count;           /* joined characters - 1 */
    /* 0x06 */ u8 slots[4];        /* character id per party slot (1-based, 0 = empty) */
    /* 0x0A */ s8 rowX[8];         /* row slide-in offsets */
} FldMenuPartyOrder; /* size 0x12 */

extern FldMenuPartyOrder fldMenuPartyOrder;
extern u8 fldMenuPartyOrderSlots[4]; /* = fldMenuPartyOrder.slots */

/* ---------------------------------------------------------------- tactics screen */

/* Tactics screen state (screen 5; FldMenuTacticsInit, 0x14 bytes). */
typedef struct FldMenuTactics {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ u8 row;
    /* 0x03 */ u8 col;
    /* 0x04 */ u8 sel;
    /* 0x05 */ u8 tactics[4][2];   /* per party slot, the two settings (PtyCharGet/SetTactics) */
    /* 0x0D */ u8 last;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s32 cur;            /* -1 after init */
} FldMenuTactics; /* size 0x14 */

extern FldMenuTactics fldMenuTactics;

/* ---------------------------------------------------------------- equip screen */

/* Equip screen state (screen 6; FldMenuEquipInit, 0x2C bytes). */
typedef struct FldMenuEquip {
    /* 0x00 */ u8 chr;             /* party slot shown */
    /* 0x01 */ u8 next;            /* party slot being switched to (L/R) */
    /* 0x02 */ u8 dir;             /* 1 switching right */
    /* 0x03 */ s8 state;           /* 0 choosing a slot, 1 choosing an item */
    /* 0x04 */ u8 pad4;
    /* 0x05 */ u8 slot;            /* equipment slot 0-5 */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 cursor;         /* item list cursor (FldMenuListCursorUp/Down) */
    /* 0x0C */ s32 top;
    /* 0x10 */ u8 help;            /* help window shown */
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ s32 last;           /* -1 forces a redraw */
    /* 0x18 */ s32 helpId;
    /* 0x1C */ s32 helpType;
    /* 0x20 */ s32 page;           /* window pair in use (0/1) */
    /* 0x24 */ u8 anim[6];         /* stat change arrows */
    /* 0x2A */ u8 pad2A[2];
} FldMenuEquip; /* size 0x2C */

extern FldMenuEquip fldMenuEquip;

/* ---------------------------------------------------------------- status screen */

/* Status screen state (screen 8; FldMenuStatusInit, 0xA8 bytes). */
typedef struct FldMenuStatus {
    /* 0x00 */ u8 chr;             /* party slot shown */
    /* 0x01 */ u8 chrNext;
    /* 0x02 */ u8 page;
    /* 0x03 */ u8 pageNext;
    /* 0x04 */ u8 dir;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ u32 cursor;
    /* 0x0C */ u32 scroll;
    /* 0x10 */ u8 timer;
    /* 0x11 */ u8 toggle;
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ s32 last;           /* -1 forces a redraw */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 help;
    /* 0x20 */ u8 winPage;         /* window pair in use (windows 8/10 + 0/1) */
    /* 0x21 */ u8 blink;
    /* 0x22 */ u8 bar[10];
    /* 0x2C */ s8 rowX[0x28];      /* row slide-in offsets */
    /* 0x54 */ f32 gauge[10];      /* resistance gauges */
    /* 0x7C */ u8 pad7C[0x2C];
} FldMenuStatus; /* size 0xA8 */

extern FldMenuStatus fldMenuStatus;

#endif /* FIELD_N23_H */
