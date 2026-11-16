#ifndef FIELD_N24_H
#define FIELD_N24_H

#include "common.h"
#include "field/menu.h"
#include "field/mini.h"

/*
 * FIELD menu screens 9-16 (0x00634408-0x00645DB0: save/load, card check,
 * junction recipes, Geo Point help, Skillman, shop, Mage junction) and
 * minigame A, the "Crash Heads Concert" rhythm game (0x00645DB0-0x0064C2E8).
 *
 * Layouts were merged from the per-file views by byte offset. Sprite and
 * quad records are FldMenuQuad (field/menu.h); memory card records are
 * McPortState / McSlot (mc.h); the minigame control block and UI records
 * are in field/mini.h.
 */

/* ---------------------------------------------------------------- save/load (screens 9-11) */

extern s32 fldMenuSaveCardSlot;     /* 0x00676330, selected card slot 0/1 */
extern s32 fldMenuMcDlgIsOpen;      /* 0x006705B0, dialog window 0xB open */
extern s8 fldMenuSaveIsLoad;        /* 0x00678C44, 0 save flow, 1 load flow */

/* ---------------------------------------------------------------- icon rects */

/* The icon tables (menuSkillTypeIcons, menuItemIcons, ...) are in sys.h. */

/* ---------------------------------------------------------------- screen 12: junction recipes */

/* Recipe grid cell being built (FldMenuJunctionRecipeCellBuild). */
typedef struct FldMenuRecipeCell {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
    /* 0x08 */ u8 alpha;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ FldMenuQuad *spr; /* window 8 sprite layer 1 */
    /* 0x10 */ FldMenuQuad *spr2; /* window 8 sprite layer 2 */
} FldMenuRecipeCell; /* size 0x14 */

typedef struct FldMenuJunctionRecipe {
    /* 0x00 */ s8 open;             /* detail window open */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u32 row;             /* cursor row (y * 48) */
    /* 0x08 */ u32 col;             /* cursor column (x * 80) */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 alpha[64];       /* per-cell alpha */
    /* 0x90 */ FldMenuRecipeCell cell;
} FldMenuJunctionRecipe; /* size 0xA4 (FldMenuJunctionRecipeInit) */

extern FldMenuJunctionRecipe fldMenuJunctionRecipe; /* 0x00678CE8 */

/* ---------------------------------------------------------------- screen 14: Skillman */

typedef struct FldMenuSkillman {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u32 page;
    /* 0x08 */ u32 cursor;
    /* 0x0C */ u32 sub;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ u8 toggle;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ s32 last;            /* -1 after init */
    /* 0x1C */ s32 help;
    /* 0x20 */ u32 timer;
    /* 0x24 */ s16 alpha24;         /* white flash alpha */
    /* 0x26 */ s16 alpha26;         /* book image alpha */
    /* 0x28 */ u16 item;            /* image index (sale id - 1) */
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s32 ripple;          /* FldMenuGetRippleWork handle */
    /* 0x30 */ f32 t;               /* bezier parameter */
    /* 0x34 */ f32 unk34;
} FldMenuSkillman; /* size 0x38 (FldMenuSkillmanInit) */

extern FldMenuSkillman fldMenuSkillman; /* 0x00678D90 */

/* ---------------------------------------------------------------- screen 15: shop */

/*
 * Shop screen work (Item Shop / Storage / Skill Books / Slot Assignment).
 * Names follow FldMenuShopUpdate (0x0063AFD8).
 */
typedef struct FldMenuShop {
    /* 0x000 */ s8 state;
    /* 0x001 */ u8 mode;            /* open types 6-9 -> 0-3 */
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ u32 sel;
    /* 0x008 */ u32 cursor;
    /* 0x00C */ u32 tab;
    /* 0x010 */ u32 cursor2;
    /* 0x014 */ u32 sel2;
    /* 0x018 */ u32 scroll;
    /* 0x01C */ u32 scroll2;
    /* 0x020 */ u8 flags[8];
    /* 0x028 */ s16 val[8];
    /* 0x038 */ u8 tim[8];
    /* 0x040 */ s8 slide[8];
    /* 0x048 */ s8 ofs[200];
    /* 0x110 */ s8 ofs2[100];
    /* 0x174 */ u8 blinkT;
    /* 0x175 */ u8 blinkDir;
    /* 0x176 */ s8 pulse[18];
    /* 0x188 */ f32 scale[8];       /* character plates */
    /* 0x1A8 */ s32 cnt[8];
    /* 0x1C8 */ f32 vel[8];
    /* 0x1E8 */ u8 pad1E8[0x14];
    /* 0x1FC */ u8 timer;
    /* 0x1FD */ u8 chr;
    /* 0x1FE */ u8 toggle;
    /* 0x1FF */ u8 pad1FF;
    /* 0x200 */ s32 last;           /* -1 after init */
    /* 0x204 */ s32 help;
    /* 0x208 */ s32 helpType;
    /* 0x20C */ s32 buf;
    /* 0x210 */ s32 nChr;           /* ChrJoinedCount */
    /* 0x214 */ u32 gold;           /* gold, or shop points in mode 2 */
    /* 0x218 */ s32 tick;
    /* 0x21C */ u8 blink;
    /* 0x21D */ u8 bar[10];
    /* 0x227 */ u8 unk227;
    /* 0x228 */ u16 ang;
    /* 0x22A */ s16 ang2;
    /* 0x22C */ u8 pad22C[4];
    /* 0x230 */ u8 win[8][0x80];
} FldMenuShop; /* size 0x630 (FldMenuShopInit) */

extern FldMenuShop fldMenuShop; /* 0x00678DD0 */

/* ---------------------------------------------------------------- screen 16: Mage junction */

/* Screen work; most fields are only used by FldMenuJunctionUpdate (0x00642478, asm). */
typedef struct FldMenuJunction {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 pad1[0x17];
    /* 0x18 */ s32 egg0;            /* chosen eggs (symbol file; types unchecked) */
    /* 0x1C */ s32 egg1;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ s32 unk24;           /* cleared by FldMenuJunctionMeshInit */
    /* 0x28 */ s32 warpTarget;      /* fldMenuJunctionWarpAmp rises toward it */
    /* 0x2C */ u8 pad2C[8];
    /* 0x34 */ u8 unk34;            /* 0x40 after init */
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 flyT;            /* fly-in bezier parameter */
    /* 0x3C */ u8 pad3C[0x24];
} FldMenuJunction; /* size 0x60 (FldMenuJunctionInit) */

extern FldMenuJunction fldMenuJunction; /* 0x00679400 */

/* Fly-in piece (FldMenuGetParticleBuf, 6 rows of 5): the four bezier
 * control points, scattered start to grid cell (x, y, scale). */
typedef struct FldMenuJunctionPiece {
    /* 0x00 */ FVECTOR pts[4];
} FldMenuJunctionPiece; /* size 0x40 */

/* ================================================================ minigame A: concert */

struct ChrObj;
struct GphAnimSet;

/* The concert's panels and sprites are the shared minigame UI's MiniPanel
 * and MiniSprite (field/mini.h). */

/* ---------------------------------------------------------------- concert data file (miniConcertData) */

typedef struct MiniConcertHdr {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
} MiniConcertHdr; /* size >= 0x16 */

/* Song record (data +0x48, five of them; miniConcertWork->song). */
typedef struct MiniConcertSong {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16[3];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 unk24[5];        /* rank thresholds (MiniConcertRankCalc) */
    /* 0x2E */ u8 pad2E[2];
} MiniConcertSong; /* size 0x30 (the song's spots follow, MiniConcertDataLoad) */

/* Camera move (data +0x14, by stage), copied to miniConcertWork->cam
 * (MiniConcertCameraInit) and played by MiniConcertCameraUpdate, which then
 * picks another one at random. */
typedef struct MiniConcertPos {
    /* 0x00 */ FVECTOR from;        /* look-at point, start */
    /* 0x10 */ FVECTOR to;          /* look-at point, end */
    /* 0x20 */ f32 dist;
    /* 0x24 */ f32 distTo;
    /* 0x28 */ f32 yaw;             /* degrees */
    /* 0x2C */ f32 yawTo;
    /* 0x30 */ f32 pitch;           /* degrees */
    /* 0x34 */ f32 pitchTo;
    /* 0x38 */ u16 id;              /* not picked twice in a row */
    /* 0x3A */ u16 unk3A;
    /* 0x3C */ u16 frames;
    /* 0x3E */ u16 unk3E;
} MiniConcertPos; /* size 0x40 */

/* Carro dance pattern (data +0x18). */
typedef struct MiniConcertPattern {
    /* 0x0 */ u8 dir[4];
    /* 0x4 */ u8 len[4];
    /* 0x8 */ u8 val[4];
    /* 0xC */ u8 at[4];
} MiniConcertPattern; /* size 0x10 */

/* Camera path (data +0x28). */
typedef struct MiniConcertCamPath {
    /* 0x00 */ f32 m[10][4];
    /* 0xA0 */ f32 ang[10];
} MiniConcertCamPath; /* size 0xC8 */

/* Scoring parameters (data +0x44). */
typedef struct MiniConcertRules {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;            /* scenario ids (EvtScnrCall) */
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 unk14[6];         /* points per rank, [2][3] */
    /* 0x1A */ u8 unk1A[3];
    /* 0x1D */ u8 unk1D;
} MiniConcertRules; /* size >= 0x1E */

typedef struct MiniConcertInfo {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 unk9;
    /* 0xA */ u8 padA[2];
} MiniConcertInfo; /* size 0xC */

typedef struct MiniConcertSpot {
    /* 0x0 */ u16 id;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
} MiniConcertSpot; /* size 0x4 */

typedef struct MiniConcertVib {
    /* 0x0 */ u16 id;
    /* 0x2 */ u8 a;
    /* 0x3 */ u8 b;
    /* 0x4 */ s32 c;
} MiniConcertVib; /* size 0x8 */

/* Parsed concert data file (MiniConcertDataLoad 0x00646810). */
typedef struct MiniConcertData {
    /* 0x00 */ MiniConcertHdr *hdr;
    /* 0x04 */ u8 *unk4;
    /* 0x08 */ u16 *unk8;
    /* 0x0C */ f32 *unkC;
    /* 0x10 */ u8 *unk10;
    /* 0x14 */ MiniConcertPos *pos;
    /* 0x18 */ MiniConcertPattern *patterns;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8 (*unk20)[3];
    /* 0x24 */ u8 (*unk24)[9];       /* formations */
    /* 0x28 */ MiniConcertCamPath *camPath;
    /* 0x2C */ u8 *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u32 unk34;
    /* 0x38 */ u32 unk38;
    /* 0x3C */ u32 unk3C;           /* panel data (MiniPanelInit) */
    /* 0x40 */ u32 unk40;
    /* 0x44 */ MiniConcertRules *rules;
    /* 0x48 */ MiniConcertSong *songs[5];
    /* 0x5C */ MiniConcertSpot *spots[5]; /* per song */
    /* 0x70 */ MiniConcertVib *vibs[5]; /* per song */
    /* 0x84 */ MiniConcertInfo info;
    /* 0x90 */ u8 pad90[4];
} MiniConcertData; /* size 0x94 */

extern MiniConcertData *miniConcertData; /* 0x006796DC */

/* ---------------------------------------------------------------- concert work (miniConcertWork) */

typedef union MiniConcertHalf {
    u16 h;
    u8 b[2];
} MiniConcertHalf;

typedef struct MiniConcertScore {
    /* 0x0 */ u16 unk0;             /* score */
    /* 0x2 */ u16 unk2;             /* combo */
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
    /* 0x8 */ u8 unk8;              /* rank */
    /* 0x9 */ u8 pad9[3];
} MiniConcertScore; /* size 0xC */

/* Song select menu (+0xB0). */
typedef struct MiniConcertMenu {
    /* 0x000 */ MiniPanel panels[2];
    /* 0x060 */ MiniSprite sprites[14];
    /* 0x5A0 */ u8 unk5A0[5];
    /* 0x5A5 */ u8 unk5A5;
    /* 0x5A6 */ u8 unk5A6;
    /* 0x5A7 */ u8 pad5A7[0xD];
} MiniConcertMenu; /* size 0x5B4 */

/* Stage setup: the 5x5 Carro roster grid (+0x664). */
typedef struct MiniConcertStage {
    /* 0x0000 */ MiniPanel panels[4];
    /* 0x00C0 */ MiniSprite sprites[64];
    /* 0x18C0 */ MiniSprite *unk18C0;
    /* 0x18C4 */ s8 unk18C4[9];
    /* 0x18CD */ s8 grid[5][5];
    /* 0x18E6 */ u8 unk18E6;
    /* 0x18E7 */ s8 unk18E7;
    /* 0x18E8 */ union {
        struct {
            s8 idx;
            s8 x;
            s8 y;
            s8 row;
        } b;
        struct {
            s32 idx : 8;
            s32 xy : 16;
            s32 pad : 8;
        } f;
    } sel;
    /* 0x18EC */ s8 unk18EC;
    /* 0x18ED */ s8 unk18ED;
    /* 0x18EE */ s8 unk18EE[5];
    /* 0x18F3 */ s8 unk18F3;
    /* 0x18F4 */ s8 unk18F4[5];
    /* 0x18F9 */ u8 unk18F9;
    /* 0x18FA */ s8 unk18FA;
    /* 0x18FB */ s8 unk18FB;
    /* 0x18FC */ u8 pad18FC[4];
} MiniConcertStage; /* size 0x1900 */

/* Stage slot / dancing Carro (+0x89F4). */
typedef struct MiniConcertActor {
    /* 0x00 */ struct ChrObj *chr;
    /* 0x04 */ MiniConcertPattern *pats[4];
    /* 0x14 */ u16 timer;
    /* 0x16 */ u8 pat;
    /* 0x17 */ u8 state;
    /* 0x18 */ u8 step;
    /* 0x19 */ s8 unk19;
    /* 0x1A */ s8 unk1A;
    /* 0x1B */ u8 sub;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ s8 unk1F;
} MiniConcertActor; /* size 0x20 */

typedef struct MiniConcertWork {
    /* 0x0000 */ MiniConcertSong *song;
    /* 0x0004 */ f32 unk4;
    /* 0x0008 */ f32 unk8;
    /* 0x000C */ f32 unkC;
    /* 0x0010 */ s32 unk10;
    /* 0x0014 */ u16 unk14;
    /* 0x0016 */ u16 unk16;
    /* 0x0018 */ u16 unk18;
    /* 0x001A */ u16 unk1A;
    /* 0x001C */ u16 unk1C;
    /* 0x001E */ u16 unk1E;
    /* 0x0020 */ u16 unk20;
    /* 0x0022 */ u8 unk22;
    /* 0x0023 */ u8 songNo;
    /* 0x0024 */ MiniConcertSpot *spots;  /* data->spots[song] */
    /* 0x0028 */ MiniConcertVib *vibs;    /* data->vibs[song] */
    /* 0x002C */ u16 unk2C;
    /* 0x002E */ u16 unk2E;
    /* 0x0030 */ MiniConcertHalf unk30;
    /* 0x0032 */ MiniConcertHalf unk32; /* points */
    /* 0x0034 */ u8 unk34;
    /* 0x0035 */ u8 unk35;
    /* 0x0036 */ u8 unk36;
    /* 0x0037 */ u8 pad37[9];
    /* 0x0040 */ u8 unk40[5];
    /* 0x0045 */ u8 unk45;
    /* 0x0046 */ u8 pad46[0xA];
    /* 0x0050 */ MiniConcertScore unk50;
    /* 0x005C */ u8 pad5C[4];
    /* 0x0060 */ MiniConcertPos cam;
    /* 0x00A0 */ f32 unkA0;
    /* 0x00A4 */ u16 frame;
    /* 0x00A6 */ u16 camFrame;      /* frames into the camera move (MiniConcertCameraUpdate) */
    /* 0x00A8 */ u8 camCount;       /* camera moves to pick from */
    /* 0x00A9 */ u8 camFirst;       /* first of them (data->pos index) */
    /* 0x00AA */ u8 camIdx;         /* camera move playing */
    /* 0x00AB */ u8 camId;          /* its id */
    /* 0x00AC */ u8 state;          /* 1 start, 2 song select, 3 stage setup, 4 play, 5 end */
    /* 0x00AD */ u8 subState;
    /* 0x00AE */ u8 padAE[2];
    /* 0x00B0 */ MiniConcertMenu menu;
    /* 0x0664 */ MiniConcertStage stage;
    /* 0x1F64 */ MiniPanel panels[4];
    /* 0x2024 */ MiniSprite sprites[281];
    /* 0x8984 */ struct ChrObj *carros[5][5];
    /* 0x89E8 */ u8 pad89E8[0xC];
    /* 0x89F4 */ MiniConcertActor actors[5];
    /* 0x8A94 */ u8 unk8A94[5][5];
    /* 0x8AAD */ u8 pad8AAD;
    /* 0x8AAE */ s16 unk8AAE[0x100]; /* debug input timing log */
    /* 0x8CAE */ u8 pad8CAE[2];
    /* 0x8CB0 */ u32 unk8CB0;       /* unk8AAE count */
    /* 0x8CB4 */ u8 pad8CB4[0xC];
} MiniConcertWork; /* size 0x8CC0 */

extern MiniConcertWork *miniConcertWork; /* 0x006796E0 */

/* ---------------------------------------------------------------- concert effects (miniConcertFx) */

typedef struct MiniConcertNote {
    /* 0x00 */ MiniSprite spr;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ u16 unk70;
    /* 0x72 */ u8 unk72;
    /* 0x73 */ u8 unk73;
    /* 0x74 */ u8 unk74;
    /* 0x75 */ u8 unk75;
    /* 0x76 */ u8 unk76;
    /* 0x77 */ u8 unk77;
    /* 0x78 */ u8 unk78;
    /* 0x79 */ u8 unk79;
    /* 0x7A */ u8 unk7A;
    /* 0x7B */ u8 pad7B[5];
} MiniConcertNote; /* size 0x80 */

typedef struct MiniConcertSpark {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u16 unk18;          /* age */
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 pad1F;
} MiniConcertSpark; /* size 0x20 */

typedef struct MiniConcertComboMark {
    /* 0x00 */ MiniSprite spr;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ u16 unk7A;
    /* 0x7C */ u8 unk7C;
    /* 0x7D */ u8 unk7D;
    /* 0x7E */ u8 pad7E[2];
} MiniConcertComboMark; /* size 0x80 */

typedef struct MiniConcertBurst {
    /* 0x00 */ MiniSprite spr;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ u16 unk78;           /* age */
    /* 0x7A */ u16 unk7A;           /* life */
    /* 0x7C */ u8 unk7C;
    /* 0x7D */ u8 unk7D;
    /* 0x7E */ u8 unk7E;
    /* 0x7F */ u8 pad7F;
} MiniConcertBurst; /* size 0x80 */

typedef struct MiniConcertFx {
    /* 0x0000 */ MiniConcertNote notes[16];
    /* 0x0800 */ MiniConcertSpark sparks[128];
    /* 0x1800 */ MiniConcertComboMark marks[128];
    /* 0x5800 */ MiniConcertBurst bursts[128];
    /* 0x9800 */ u8 unk9800;        /* draw flags */
    /* 0x9801 */ u8 unk9801;
    /* 0x9802 */ u8 unk9802;
    /* 0x9803 */ u8 pad9803[0xD];
} MiniConcertFx; /* size 0x9810 */

extern MiniConcertFx *miniConcertFx; /* 0x006796E4 */

/* ---------------------------------------------------------------- shared UI globals */

/* miniUi / miniLayout under the concert's own symbols. */
extern MiniUi *miniConcertUi;         /* 0x006796D8 */
extern MiniLayout *miniConcertLayout; /* 0x006796D4 */

#endif /* FIELD_N24_H */
