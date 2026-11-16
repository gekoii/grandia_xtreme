#ifndef FIELD_MINI_H
#define FIELD_MINI_H

/*
 * The minigame control block and the shared minigame UI (Mini*
 * 0x0064C2E8-0x0064F8C0): layout resources, panels, sprites, text slots,
 * input and sparkles. Minigame A (the concert, field/n24.h) and minigame B
 * (the ball game, field/n25.h) both use them.
 */

#include "common.h"
#include "gph.h"
#include "sys.h"

struct ChrObj;

/* Minigame control (0x70): MiniInit 0x00645DB0, MiniLoad 0x00645E00, MiniStart 0x00645F58. */
typedef struct MiniCtl {
    /* 0x00 */ u8 kind;             /* 0/1 concert, 2 ball */
    /* 0x01 */ u8 state;
    /* 0x02 */ u16 startEvt;        /* event ids run at start / end */
    /* 0x04 */ u16 endEvt;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 dataFile;
    /* 0x0C */ struct ChrObj *carros[5][5]; /* Carro tenants */
} MiniCtl; /* size 0x70 */

extern MiniCtl *miniCtl;            /* 0x006796D0 */

/* Layout resource header (MiniLayoutLoad 0x0064C380; minigame B copies its
 * own into MiniBallData.layoutHdr). Counts of the parts that follow. */
typedef struct MiniLayoutHdr {
    /* 0x0 */ s32 msgSize;        /* message bank bytes */
    /* 0x4 */ u8 groupCount;      /* MiniMsgGroup entries */
    /* 0x5 */ u8 rectCount;       /* SprRect entries */
    /* 0x6 */ u8 idxCount;        /* rect list index bytes */
    /* 0x7 */ u8 listCount;       /* MiniRectList entries */
    /* 0x8 */ u8 imageCount;      /* textures + anim sets */
    /* 0x9 */ u8 texCount;        /* the first texCount images are plain textures, the rest anim sets */
    /* 0xA */ u8 padA[2];
} MiniLayoutHdr; /* size 0xC */

/* Message group: lines first..first+count-1 (MiniSpriteSetMsgGroup, MiniMsgBoxSet). */
typedef struct MiniMsgGroup {
    /* 0x0 */ u8 first;
    /* 0x1 */ u8 count;
} MiniMsgGroup; /* size 0x2 */

/* Rect list (SprDrawRectList); rect is an index into the rects, relocated to
 * a pointer (kept s32: the SprRectListEntry of the loaded file). */
typedef struct MiniRectList {
    /* 0x0 */ s32 rect;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
} MiniRectList; /* size 0x8 */

/* Layout resource as loaded by MiniLayoutLoad (miniLayout, 0x24 bytes). */
typedef struct MiniLayout {
    /* 0x00 */ MiniLayoutHdr *hdr;
    /* 0x04 */ MiniMsgGroup *groups;
    /* 0x08 */ u8 *msgs;          /* message bank (TxtMsgHdr) */
    /* 0x0C */ SprRect *rects;
    /* 0x10 */ u8 *listIdx;       /* rect -> rect list index */
    /* 0x14 */ MiniRectList *lists;
    /* 0x18 */ s32 *images;       /* texture / anim set pointers (relocated offsets; the concert
                                   * casts them to GphAnimSet **) */
    /* 0x1C */ u8 *imageData;
    /* 0x20 */ u8 pad20[4];
} MiniLayout; /* size 0x24 (MiniUiAlloc) */

/* Initial sprite placement of a panel's cells (8 bytes, MiniSpriteInit). */
typedef struct MiniCell {
    /* 0x0 */ u16 ofsX;
    /* 0x2 */ u16 ofsY;
    /* 0x4 */ u8 rect;            /* >= 100: rect list rect - 100 */
    /* 0x5 */ u8 active;
    /* 0x6 */ u8 fxFlags;
    /* 0x7 */ u8 layer;
} MiniCell; /* size 0x8 */

/* Panel description (MiniPanelInit), 0x20-byte stride in the data file. */
typedef struct MiniPanelSrc {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 cellW;         /* passed to MiniSpriteInit as u8 */
    /* 0x0C */ f32 cellH;
    /* 0x10 */ u8 first;          /* first and last cell/sprite */
    /* 0x11 */ u8 last;
    /* 0x12 */ u8 topLeft;
    /* 0x13 */ u8 tex;
    /* 0x14 */ u8 slide;          /* MiniPanelSlideIn direction */
    /* 0x15 */ u8 pad15[0xB];
} MiniPanelSrc; /* size 0x20 */

struct MiniSprite;

/* Text or number slot of a sprite (miniUi->texts / nums). */
typedef struct MiniTextSlot {
    /* 0x00 */ struct MiniSprite *owner;
    /* 0x04 */ u8 w;              /* glyph size (16 = 1:1); s8 in 64CDC8 */
    /* 0x05 */ u8 h;
    /* 0x06 */ u8 visible;
    /* 0x07 */ s8 msg;            /* message number, -1 for a string */
    /* 0x08 */ u8 id;             /* glyph queue row */
    /* 0x09 */ u8 len;            /* glyphs; digits for a number */
    /* 0x0A */ u8 align;
    /* 0x0B */ u8 padB;
    /* 0x0C */ u32 value;         /* number slots */
    /* 0x10 */ u8 mode;           /* 0 message, 1 string */
    /* 0x11 */ u8 count;          /* glyphs drawn */
    /* 0x12 */ u8 gapOn;          /* number slot: leave a gap in the text */
    /* 0x13 */ u8 gapAt;
    /* 0x14 */ u8 gapWidth;
    /* 0x15 */ u8 pad15[7];
    /* 0x1C */ u8 *str;
    /* 0x20 */ u32 width;         /* s32 in 64C838/64CAB8 (stores) */
} MiniTextSlot; /* size 0x24 */

/* MiniSprite.moveFlags */
#define MINI_MOVE_POS    0x1
#define MINI_MOVE_ANIM   0x2  /* animNo = rect at the end */
#define MINI_MOVE_SCALE  0x4
#define MINI_MOVE_HIDE   0x8  /* active = 0 at the end */

/* MiniSprite.drawFlags */
#define MINI_DRAW_RECT    0x1
#define MINI_DRAW_ANIM    0x2
#define MINI_DRAW_TEXT    0x4
#define MINI_DRAW_NUMBER  0x8
#define MINI_DRAW_SPARKLE 0x10

/* Minigame sprite (MiniSpriteInit 0x0064C570, MiniSpritesUpdate 0x0064D0B8,
 * MiniPanelsDraw 0x0064DC60). */
typedef struct MiniSprite {
    /* 0x00 */ f32 sx;
    /* 0x04 */ f32 sy;
    /* 0x08 */ s32 angle;
    /* 0x0C */ s16 x;             /* u16 in 64C6B8 */
    /* 0x0E */ s16 y;
    /* 0x10 */ s16 ofsX;          /* u16 in 64C570 */
    /* 0x12 */ s16 ofsY;
    /* 0x14 */ s16 goalX;
    /* 0x16 */ s16 goalY;
    /* 0x18 */ s16 startX;
    /* 0x1A */ s16 startY;
    /* 0x1C */ s8 cellX;          /* cursor cell (MiniCursorMove) */
    /* 0x1D */ s8 cellY;
    /* 0x1E */ u8 cellW;
    /* 0x1F */ u8 cellH;
    /* 0x20 */ u8 r;
    /* 0x21 */ u8 g;
    /* 0x22 */ u8 b;
    /* 0x23 */ u8 a;
    /* 0x24 */ u16 moveCount;     /* s16 in the setters */
    /* 0x26 */ u16 moveTime;
    /* 0x28 */ u16 frame;
    /* 0x2A */ u8 layer;
    /* 0x2B */ u8 active;
    /* 0x2C */ u8 moveFlags;      /* MINI_MOVE_* */
    /* 0x2D */ u8 fxFlags;        /* 2 pulse, 4/8 wave x/y, 0x10 stretch, 0x20 bounce, 0x40 shrink, 0x80 blink */
    /* 0x2E */ u8 topLeft;        /* 0: x/y is the centre */
    /* 0x2F */ u8 drawFlags;      /* MINI_DRAW_* */
    /* 0x30 */ u8 pad30[2];
    /* 0x32 */ u8 alphaScale;     /* 0x80 = 1.0; s8 in 64D0B8 */
    /* 0x33 */ u8 blinkPeriod;
    /* 0x34 */ s8 waveT;
    /* 0x35 */ s8 wavePeriod;
    /* 0x36 */ s8 waveAmpX;
    /* 0x37 */ s8 waveAmpY;
    /* 0x38 */ s8 waveX;
    /* 0x39 */ s8 waveY;
    /* 0x3A */ u16 stretchT;      /* s16 in 64C570 */
    /* 0x3C */ s16 stretchSpeed;
    /* 0x3E */ u8 stretchRect;    /* s8 in 64C570 */
    /* 0x3F */ u8 stretchMode;    /* 0 u, 1 v, 2 angle */
    /* 0x40 */ s8 pulseX;         /* percent per frame; u8 in 64C570 */
    /* 0x41 */ s8 pulseY;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ MiniTextSlot *text;
    /* 0x48 */ s16 textOfsX;
    /* 0x4A */ s16 textOfsY;
    /* 0x4C */ MiniTextSlot *num;
    /* 0x50 */ s16 numOfsX;
    /* 0x52 */ s16 numOfsY;
    /* 0x54 */ u8 rect;           /* layout rect (or icon) */
    /* 0x55 */ u8 rectIsList;
    /* 0x56 */ u8 icon;           /* menu icon texture, 0 none */
    /* 0x57 */ u8 animNo;         /* anim set (1-based), 0 none */
    /* 0x58 */ GphAnimState anim;
} MiniSprite; /* size 0x60 */

/* The cursor cell (cellX, cellY) as one halfword: 0x0064B308/0x0064B780
 * compare two sprites' cells with one lhu each. */
#define MINI_SPRITE_CELL(o) (*(u16 *)&(o)->cellX)

/* MiniPanel.flags */
#define MINI_PANEL_MOVE     0x1
#define MINI_PANEL_FADE_IN  0x2
#define MINI_PANEL_FADE_OUT 0x4

/* A group of sprites drawn with one texture (MiniPanelInit 0x0064C6B8,
 * MiniPanelsUpdate 0x0064D8F0, MiniPanelsDraw 0x0064DC60). */
typedef struct MiniPanel {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 sx;
    /* 0x0C */ f32 sy;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 baseX;
    /* 0x16 */ s16 baseY;
    /* 0x18 */ s16 goalX;
    /* 0x1A */ s16 goalY;
    /* 0x1C */ s16 startX;
    /* 0x1E */ s16 startY;
    /* 0x20 */ u16 moveCount;     /* s16 in 64C6B8/64DA90/64DBF8 */
    /* 0x22 */ u16 moveTime;
    /* 0x24 */ MiniSprite *sprites;
    /* 0x28 */ u8 count;
    /* 0x29 */ u8 active;
    /* 0x2A */ u8 flags;          /* MINI_PANEL_* */
    /* 0x2B */ u8 maxLayer;
    /* 0x2C */ u8 tex;            /* layout image */
    /* 0x2D */ u8 showNumbers;
    /* 0x2E */ u8 showIcons;
    /* 0x2F */ u8 alpha;
} MiniPanel; /* size 0x30 */

/* Pad state of the minigames (MiniInputUpdate 0x0064F3A0), miniUi->input. */
typedef struct MiniInput {
    /* 0x00 */ u16 held;
    /* 0x02 */ u16 trigger;
    /* 0x04 */ u16 repeat;
    /* 0x06 */ s8 dx;             /* read as one u16 with dy by MiniCursorMove (MINI_INPUT_DIR) */
    /* 0x07 */ s8 dy;
    /* 0x08 */ u8 stickMoved;
    /* 0x09 */ s8 lx;
    /* 0x0A */ s8 ly;
    /* 0x0B */ u8 padB;
    /* 0x0C */ u8 circleWait;     /* button cool-downs (5 frames) */
    /* 0x0D */ u8 triangleWait;
    /* 0x0E */ u8 crossWait;
    /* 0x0F */ u8 squareWait;
    /* 0x10 */ u8 buttonWait;
    /* 0x11 */ u8 button;         /* last button: 0 circle, 1 triangle, 2 cross, 3 square */
    /* 0x12 */ u8 stickFrames;
    /* 0x13 */ u8 stickWait;
    /* 0x14 */ s8 maxX;
    /* 0x15 */ s8 minX;
    /* 0x16 */ s8 maxY;
    /* 0x17 */ s8 minY;
    /* 0x18 */ s8 pushX;          /* stick flick */
    /* 0x19 */ s8 pushY;
    /* 0x1A */ u8 stickActive;    /* read as u16 with stickFlick by MiniBallStickPush (MINI_INPUT_STICK) */
    /* 0x1B */ u8 stickFlick;
} MiniInput; /* size 0x1C */

/* Byte pairs of MiniInput tested as one halfword: dx/dy (MiniCursorMove: no
 * direction pressed) and stickActive/stickFlick (MiniBallStickPush: 1 is
 * active without a flick). */
#define MINI_INPUT_DIR(in)   (*(u16 *)&(in)->dx)
#define MINI_INPUT_STICK(in) (*(u16 *)&(in)->stickActive)

/* Minigame UI work (miniUi, MiniUiAlloc: 0x4D0 bytes). */
typedef struct MiniUi {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s8 curTex;        /* texture bound for the layout, -1 none (MiniUiResetTexCache) */
    /* 0x011 */ u8 pad11[0xF];
    /* 0x020 */ MiniTextSlot texts[16];
    /* 0x260 */ MiniTextSlot nums[16];
    /* 0x4A0 */ MiniInput input;
    /* 0x4BC */ u8 pad4BC[0x14];
} MiniUi; /* size 0x4D0 */

/* Sparkle (MiniSparkleSpawn/Update/Draw). */
typedef struct MiniSparkle {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 speed;
    /* 0xC */ u8 timer;           /* alpha while < 0x80 */
    /* 0xD */ u8 padD;
    /* 0xE */ s16 angle;          /* u16 in 64F130 */
} MiniSparkle; /* size 0x10 */

extern MiniSparkle miniSparkles[32];  /* 0x006796E8 */
extern s8 miniSparkleIndex;           /* 0x006798E8, -1 none */
extern MiniLayout *miniLayout;        /* 0x006798EC */
extern MiniUi *miniUi;                /* 0x006798F0 */

#endif /* FIELD_MINI_H */
