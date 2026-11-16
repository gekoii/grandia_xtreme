#ifndef FIELD_N25_H
#define FIELD_N25_H

/*
 * FIELD minigames (0x0064C2E8-0x00653AE0), world map (Wmap*, 0x00653AE0-
 * 0x00656910), title/attract (FldTitle*), picture display (FldPicture*),
 * flag list window (FldFlagList*) and the sprite effect pool (FldSprite*,
 * slots in ptcl.h).
 *
 * Layouts were merged from the per-file views by byte offset.
 */

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "field/mini.h"

/* ================================================================ minigames */

/* The shared minigame UI (layouts, panels, sprites, input) is in field/mini.h. */

/* ---------------------------------------------------------------- minigame B */

/* Data file header (MiniBallDataLoad 0x0064FE50). +0x0/+0x4/+0x8..+0xC are
 * copied to MiniBallData.layoutHdr. */
typedef struct MiniBallHdr {
    /* 0x0 */ s32 msgSize;
    /* 0x4 */ u8 groupCount;
    /* 0x5 */ u8 wallCount;
    /* 0x6 */ u8 panelCount;
    /* 0x7 */ u8 cellCount;
    /* 0x8 */ u8 rectCount;
    /* 0x9 */ u8 idxCount;
    /* 0xA */ u8 listCount;
    /* 0xB */ u8 imageCount;
    /* 0xC */ u8 texCount;
    /* 0xD */ u8 courseCount;
    /* 0xE */ u8 setCount;        /* rule sets (MiniBallSet) */
    /* 0xF */ u8 padF;
} MiniBallHdr; /* size 0x10 */

/* Course (8 bytes), followed in the file by probCount {f32, u8 set} pairs. */
typedef struct MiniBallCourse {
    /* 0x0 */ s32 cost;           /* gold per ball */
    /* 0x4 */ u8 prizeRank;       /* AmapRollItemsByRank */
    /* 0x5 */ u8 probCount;
    /* 0x6 */ u8 defaultSet;
    /* 0x7 */ u8 minLevel;        /* MiniBallSetRank */
} MiniBallCourse; /* size 0x8 */

/* Rule set chosen by MiniBallCourseSelect (0x18 bytes, then a 0x24-byte
 * block (params2) and gustCount MiniBallGust entries). */
typedef struct MiniBallSet {
    /* 0x00 */ s32 bgm;
    /* 0x04 */ u32 pushEnd;       /* last frame of MiniBallStickPush / MiniBallRandomGusts */
    /* 0x08 */ u32 shootEnd;      /* last frame of MiniBallShootInput */
    /* 0x0C */ u32 pickTime;      /* MiniBallPickWinner */
    /* 0x10 */ u32 judgeTime;     /* MiniBallJudge */
    /* 0x14 */ u8 gustMode;       /* 2: scripted gusts */
    /* 0x15 */ u8 gustCount;
    /* 0x16 */ u8 pad16[2];
} MiniBallSet; /* size 0x18 */

/* Scripted gust (MiniBallScriptedGusts). The first one's start/len also
 * seed gustWait/gustLeft. */
typedef struct MiniBallGust {
    /* 0x0 */ u16 start;
    /* 0x2 */ u16 len;
    /* 0x4 */ f32 x;
    /* 0x8 */ f32 y;
    /* 0xC */ f32 unkC;
} MiniBallGust; /* size 0x10 */

/* Tuning block of the data file (0x58 bytes before the layout resource). */
typedef struct MiniBallParams {
    /* 0x00 */ f32 ballNormalK;   /* ball-ball bounce, along the normal (MiniBallCollideBalls) */
    /* 0x04 */ f32 ballTangentK;  /* ball-ball bounce, along the contact side */
    /* 0x08 */ f32 wallNormalK;   /* wall bounce (MiniBallBounceWall); also scales the travel left */
    /* 0x0C */ f32 wallTangentK;
    /* 0x10 */ f32 pullZ;         /* added to vel z each frame (MiniBallBallsUpdate) */
    /* 0x14 */ f32 gustX;         /* MiniBallRandomGusts */
    /* 0x18 */ f32 gustY;
    /* 0x1C */ f32 gustBiasY;
    /* 0x20 */ f32 launchX;       /* MiniBallLaunch */
    /* 0x24 */ f32 launchZ;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ f32 launchVelZ;
    /* 0x30 */ f32 launchStepX;
    /* 0x34 */ u8 pad34[4];
    /* 0x38 */ f32 dropX;         /* lane x once a launched ball turns back (MiniBallBallsUpdate) */
    /* 0x3C */ f32 dropZ;         /* z where it starts rolling */
    /* 0x40 */ f32 winX;          /* the winning ball is placed here (MiniBallPickWinner) */
    /* 0x44 */ f32 winZ;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ f32 winEndZ;       /* z where the winning ball rolls again */
    /* 0x50 */ u16 entryScnr;     /* MiniBallEntryUpdate */
    /* 0x52 */ u16 setupScnr;     /* MiniBallSetupUpdate */
    /* 0x54 */ u8 pad54[4];
} MiniBallParams; /* size 0x58 */

/* Wall segment (MiniBallDebugDraw draws them as lines). */
typedef struct MiniBallWall {
    /* 0x0 */ s16 x0;
    /* 0x2 */ s16 y0;
    /* 0x4 */ s16 x1;
    /* 0x6 */ s16 y1;
} MiniBallWall; /* size 0x8 */

/* Parsed data file (miniBallData, MiniBallAlloc: 0x44 bytes). */
typedef struct MiniBallData {
    /* 0x00 */ MiniBallHdr *hdr;
    /* 0x04 */ MiniBallCourse *courses;
    /* 0x08 */ MiniBallSet *set;
    /* 0x0C */ u32 sets;          /* address of the first rule set */
    /* 0x10 */ u16 *reelTimes;    /* 3 per reel position (MiniBallReelSetTiming) */
    /* 0x14 */ MiniBallGust *gusts;
    /* 0x18 */ MiniBallWall *walls;
    /* 0x1C */ MiniPanelSrc *panels;
    /* 0x20 */ MiniCell *cells;
    /* 0x24 */ MiniBallParams *params;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ MiniLayoutHdr layoutHdr;
    /* 0x40 */ u8 pad40[4];
} MiniBallData; /* size 0x44 */

/* Ball (0x40, MiniBallLaunch 0x00651C68, MiniBallMove 0x006525C8). */
typedef struct MiniBall {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ FVECTOR vel;
    /* 0x20 */ f32 travel;         /* distance left to move this frame (MiniBallMove) */
    /* 0x24 */ f32 speed;
    /* 0x28 */ MiniSprite *sprite;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u32 step;           /* collision steps this frame */
    /* 0x34 */ u8 state;          /* 1 launched, 2 falling back, 3 landing, 4 rolling, 5 winner */
    /* 0x35 */ u8 index;          /* launch order */
    /* 0x36 */ u8 color;
    /* 0x37 */ s8 lastWall;       /* wall hit in this frame's steps, -1 none */
    /* 0x38 */ s8 lastBall;       /* ball hit, -1 none */
    /* 0x39 */ u8 pad39[7];
} MiniBall; /* size 0x40 */

/* Nearest hit found by MiniBallSweepWalls / MiniBallSweepBalls, applied by
 * MiniBallBounceWall (a stack record of MiniBallMoveStep). */
typedef struct MiniBallHit {
    /* 0x00 */ FVECTOR side;      /* along the wall / contact */
    /* 0x10 */ FVECTOR normal;
    /* 0x20 */ FVECTOR move;      /* movement up to the hit */
    /* 0x30 */ f32 dist;
    /* 0x34 */ s32 index;         /* wall or ball */
    /* 0x38 */ s32 kind;          /* 0 wall, 1 ball, 2 none (MiniBallMoveStep) */
    /* 0x3C */ u8 pad3C[4];
} MiniBallHit; /* size 0x40 */

/* Prize slot (MiniBallRollPrizes 0x00651048). */
typedef struct MiniBallPrize {
    /* 0x00 */ s16 itemId;        /* u16 in 651048/651120 */
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 icon;
    /* 0x08 */ s32 name;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ u16 times[3];      /* reel timing (MiniBallReelSetTiming): start, slow-down, stop frame */
    /* 0x14 */ u16 nextStep;      /* frame of the next reel step (MiniBallReelsUpdate) */
    /* 0x16 */ u8 spinning;       /* set at times[0], or early by MiniBallJudge */
    /* 0x17 */ u8 stepLen;        /* frames per step, +5 after times[1] */
    /* 0x18 */ u8 pos;            /* reel position shown */
    /* 0x19 */ u8 reel;           /* reel position */
    /* 0x1A */ u8 pad1A[6];
} MiniBallPrize; /* size 0x20 */

/* Board part of the minigame B work (MiniBallWork.game). Most of the
 * MiniBall* functions take &miniBallWork->game as their base pointer. */
typedef struct MiniBallGame {
    /* 0x0000 */ MiniPanel panels[10];
    /* 0x01E0 */ MiniSprite sprites[63];
    /* 0x1980 */ MiniBall balls[20];
    /* 0x1E80 */ u32 cost;        /* per ball; s32 in 6500D8 */
    /* 0x1E84 */ u8 launched;
    /* 0x1E85 */ u8 reelsStopped;
    /* 0x1E86 */ u8 judged;
    /* 0x1E87 */ u8 winColor;
    /* 0x1E88 */ u8 ballHitSe;     /* play 0x813 (set by MiniBallCollideBalls) */
    /* 0x1E89 */ u8 ballHitVol;    /* quietest pending hit, 0xFF none */
    /* 0x1E8A */ u8 ballHitWait;
    /* 0x1E8B */ u8 wallHitSe;     /* play 0x80A (set by MiniBallBounceWall) */
    /* 0x1E8C */ u8 wallHitVol;
    /* 0x1E8D */ u8 wallHitWait;
    /* 0x1E8E */ u8 pad1E8E[2];
    /* 0x1E90 */ u32 gold;        /* s32 in 64F8C0/650680 */
    /* 0x1E94 */ u8 pad1E94;
    /* 0x1E95 */ u8 course;       /* 1-based */
    /* 0x1E96 */ u8 color;        /* chosen colour */
    /* 0x1E97 */ u8 reelsDone;     /* reels stopped (MiniBallReelsUpdate) */
    /* 0x1E98 */ u8 reelOrder[6];
    /* 0x1E9E */ u8 pad1E9E[2];
    /* 0x1EA0 */ MiniBallPrize prizes[6];
    /* 0x1F60 */ FVECTOR shakePos; /* board shake spring (MiniBallShakeUpdate) */
    /* 0x1F70 */ FVECTOR shakeVel;
    /* 0x1F80 */ FVECTOR tilt;     /* pushes (MiniBallStickPush, x and z), taken by the spring each frame */
    /* 0x1F90 */ u16 gustLeft;
    /* 0x1F92 */ u16 gustWait;
    /* 0x1F94 */ u8 gustIndex;
    /* 0x1F95 */ u8 pad1F95[3];
    /* 0x1F98 */ MiniSprite *msgBox;
    /* 0x1F9C */ u8 chooseMode;
    /* 0x1F9D */ u8 pad1F9D[3];
} MiniBallGame; /* size 0x1FA0 */

/* Minigame B work (miniBallWork, MiniBallAlloc: 0x1FD0 bytes). */
typedef struct MiniBallWork {
    /* 0x0000 */ FVECTOR vec0;    /* copied as a blank vector (velocities, side vectors) */
    /* 0x0010 */ u32 frame;       /* frames in the current step; s32 in several views */
    /* 0x0014 */ union {
                     u16 n;
                     u8 b[2];
                 } plays;         /* play count, saved as two bytes */
    /* 0x0016 */ u8 state;        /* 1 wait, 2 entry, 3 setup, 4 play, 5 end */
    /* 0x0017 */ u8 subState;
    /* 0x0018 */ u8 pad18;
    /* 0x0019 */ s8 dbgBall;      /* MiniBallDebugDraw, -1 none */
    /* 0x001A */ u8 prizeStored;  /* the prize went to storage */
    /* 0x001B */ u8 pad1B[0x15];
    /* 0x0030 */ MiniBallGame game;
} MiniBallWork; /* size 0x1FD0 */

extern MiniLayout *miniBallLayout;    /* 0x006798F4 */
extern MiniUi *miniBallUi;            /* 0x006798F8 */
extern MiniBallData *miniBallData;    /* 0x006798FC */
extern MiniBallWork *miniBallWork;    /* 0x00679900 */

/* ================================================================ world map */

/* Destination node (0x50; WmapInit 0x00653C08 sets up six). */
typedef struct WmapNode {
    /* 0x00 */ u16 id;            /* s16 in 653C08 */
    /* 0x02 */ u16 mapNo;
    /* 0x04 */ u16 entrance;
    /* 0x06 */ u8 unlocked;
    /* 0x07 */ u8 state;          /* 1 linked to the current node, 2 other */
    /* 0x08 */ s16 links[8];      /* node per direction, 0xFF none */
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ FVECTOR pos;       /* marker */
    /* 0x30 */ FVECTOR camPos;
    /* 0x40 */ FVECTOR camTarget;
} WmapNode; /* size 0x50 */

/* World map work (wmapWork, WmapInit: 0x300 bytes). */
typedef struct WmapWork {
    /* 0x000 */ WmapNode nodes[8];
    /* 0x280 */ FVECTOR markerPos;    /* map object 4 position (deform centre) */
    /* 0x290 */ f32 unk290;
    /* 0x294 */ f32 unk294;
    /* 0x298 */ f32 unk298;
    /* 0x29C */ f32 unk29C;           /* 0..360 */
    /* 0x2A0 */ f32 unk2A0;
    /* 0x2A4 */ f32 unk2A4;
    /* 0x2A8 */ u8 pad2A8[8];
    /* 0x2B0 */ FVECTOR cursorVel;    /* WmapVecApproach state (WmapControl) */
    /* 0x2C0 */ FVECTOR camPosVel;
    /* 0x2D0 */ FVECTOR camTargetVel;
    /* 0x2E0 */ f32 unk2E0;
    /* 0x2E4 */ f32 waveT;            /* terrain wave, 0..360 */
    /* 0x2E8 */ f32 warpT;            /* WmapWarpCalcGrid */
    /* 0x2EC */ u8 pad2EC;
    /* 0x2ED */ u8 active;
    /* 0x2EE */ u16 cur;              /* selected node */
    /* 0x2F0 */ s32 mode;             /* 0 control, 1 entering, 2 ripples */
    /* 0x2F4 */ s32 eventDone;
    /* 0x2F8 */ u32 frame;            /* s32 in 654058 */
    /* 0x2FC */ u8 pad2FC[4];
} WmapWork; /* size 0x300 */

typedef struct WmapRipple {
    /* 0x00 */ s32 active;
    /* 0x04 */ f32 t;
    /* 0x08 */ f32 end;
    /* 0x0C */ s32 padC;
    /* 0x10 */ FVECTOR pos;
} WmapRipple; /* size 0x20 */

/* Ripple rings (wmapRipples, WmapInit: 0x210 bytes). */
typedef struct WmapRipples {
    /* 0x000 */ u8 flags;         /* 1 spawn */
    /* 0x001 */ u8 count;         /* 10 */
    /* 0x002 */ u16 timer;        /* s16 in 653AF0 */
    /* 0x004 */ u16 period;
    /* 0x006 */ u8 pad6[0xA];
    /* 0x010 */ WmapRipple rings[16];
} WmapRipples; /* size 0x210 */

extern WmapWork *wmapWork;            /* 0x00679920 */
extern WmapRipples *wmapRipples;      /* 0x00679924 */
extern FVECTOR wmapCursorPos;         /* 0x00679950 */
extern FVECTOR wmapCursorGoal;        /* 0x00679960 */
extern FVECTOR wmapCamTarget;         /* 0x00679970 */
extern FVECTOR wmapCamTargetGoal;     /* 0x00679980 */
extern FVECTOR wmapCamPos;            /* 0x00679990 */
extern FVECTOR wmapCamPosGoal;        /* 0x006799A0 */
extern FVECTOR wmapCursorTrail[4];    /* 0x006799B0 */
extern s32 wmapEntering;              /* 0x006799F0 */
extern void *wmapWarpGrid;            /* 0x006799F4 */
extern void *wmapWarpPacket;          /* 0x006799F8 */

/* ============================================================ title, picture */

/* Title / attract work (fldTitleWork, FldTitleInit: 0x7C0 bytes). */
typedef struct FldTitleWork {
    /* 0x000 */ u8 shot;          /* camera shot played */
    /* 0x001 */ u8 shotNext;
    /* 0x002 */ u8 unk2;
    /* 0x003 */ u8 cursor;        /* menu cursor (0 new game, 1, 2) */
    /* 0x004 */ u8 started;
    /* 0x005 */ u8 blink;         /* 0..0x77, prompt shown from 9 (FldTitleDraw) */
    /* 0x006 */ s8 order[8];      /* shot order */
    /* 0x00E */ u8 padE[2];
    /* 0x010 */ s32 frame;        /* FldTitleCamFixedUpdate */
    /* 0x014 */ f32 t;            /* along the shot, 0..1 */
    /* 0x018 */ u8 pad18[0x28];
    /* 0x040 */ GsTexPacket logoTex;  /* GsTexLoadPacket */
    /* 0x180 */ GphBatchQuad quads[16]; /* menu and caption prims (FldTitleDraw) */
    /* 0x580 */ u8 pad580[0x200];
    /* 0x780 */ FVECTOR *shotKeys;  /* 8 keys per shot, 64 per map set; s32 in 656948 */
    /* 0x784 */ u8 pad784[0x3C];
} FldTitleWork; /* size 0x7C0 */

/* Picture display work (fldPictureWork, FldPictureInit: 0x100 bytes). */
typedef struct FldPictureWork {
    /* 0x00 */ u8 shown;
    /* 0x01 */ u8 req;
    /* 0x02 */ u8 slots[2];       /* picture per slot (1-based): [0] current, [1] leaving */
    /* 0x04 */ s16 dim;           /* background darkening 0..0x80 */
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ GphBatchQuad prim; /* batch quad / sprite (FldPictureDraw) */
    /* 0x50 */ s32 primEnd;       /* terminator (next quad's type) */
    /* 0x54 */ u8 pad54[0x3C];
    /* 0x90 */ f32 t[2];          /* s32 in 6577C0 (cleared) */
    /* 0x98 */ u8 pad98[8];
    /* 0xA0 */ FVECTOR out[2];    /* x, y, alpha per slot */
    /* 0xC0 */ FVECTOR keys[4];   /* Bezier keys */
} FldPictureWork; /* size 0x100 */

extern FldTitleWork *fldTitleWork;     /* 0x00679D00 */
extern FldPictureWork *fldPictureWork; /* 0x00679D04 */

/* ============================================================ flag list, sprites */

/* Flag list window work (fldFlagListWork, 0x6460 bytes). */
typedef struct FldFlagListWork {
    /* 0x00 */ u16 open;
    /* 0x02 */ u16 event;
    /* 0x04 */ u16 cursor;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 page;
    /* 0x0A */ u8 padA[4];
    /* 0x0E */ u8 entries[0x40];  /* fldFlagListEntries index per line */
    /* 0x4E */ u16 count;
    /* 0x50 */ u16 pages;
    /* 0x52 */ u8 pad52[0xE];
    /* 0x60 */ u8 winSprites[0x6400]; /* FldMenuWinSetLayer */
} FldFlagListWork; /* size 0x6460 */

/* Sprite effect textures and draw packets (fldSpriteWork, FldSpriteInit: 0x27C0 bytes). */
typedef struct FldSpriteWork {
    /* 0x000 */ u8 texPackets[9][0x140];  /* one per kind */
    /* 0xB40 */ u8 counts[16];            /* live sprites per kind */
    /* 0xB50 */ u8 quads[65][0x70];       /* 3D quad packets (GphBatchDrawQuads3D) */
} FldSpriteWork; /* size 0x27C0 */

extern FldFlagListWork *fldFlagListWork; /* 0x00679D08 */
extern FldSpriteWork *fldSpriteWork;     /* 0x00679D10 */

#endif /* FIELD_N25_H */
