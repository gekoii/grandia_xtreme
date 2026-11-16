#ifndef FIELD_N15_H
#define FIELD_N15_H

#include "common.h"
#include "vu.h"
#include "camera.h"
#include "field/state.h"   /* ChrEnemyWork */

/*
 * FIELD 0x005C95E8-0x005D3758: camera blend, follow camera, camera trail,
 * camera scripts, the message page cursor, the enemy work pool, the map
 * load state machine and the animated ripple textures.
 *
 * The field cameras themselves (camField, camCtl) are in field/state.h.
 */

/* ---------------------------------------------------------------- camBlend */

/* Camera blend (CamBlendStart 0x005C9FD0, CamBlendUpdate 0x005CA1A8):
 * Bezier from `from` to `to` over `duration` frames. */
typedef struct CamBlend {
    /* 0x000 */ Camera from;
    /* 0x180 */ Camera to;         /* camBlendTo */
    /* 0x300 */ f32 unk300;
    /* 0x304 */ f32 unk304;
    /* 0x308 */ f32 t;             /* 0..1 (s32 zero store in 0x005C9FD0) */
    /* 0x30C */ f32 duration;      /* frames */
    /* 0x310 */ u8 active;         /* camBlendActive */
    /* 0x311 */ u8 pad311[0xF];
} CamBlend; /* size 0x320 (CamBlendInit) */

extern CamBlend camBlend;
extern Camera camBlendTo;          /* = camBlend.to */
extern u8 camBlendActive;          /* = camBlend.active */

extern s32 camShakeInterval;       /* frames between shake updates */
extern s32 camShakeCountdown;

/* ---------------------------------------------------------------- follow camera */

/* Camera state the follow AI works on (CamStateCalcDir 0x005CACF0). */
typedef struct CamState {
    /* 0x00 */ FVECTOR lookAt;     /* Camera.lookAt */
    /* 0x10 */ FVECTOR eye;        /* Camera.eye */
    /* 0x20 */ FVECTOR dir;        /* from pitch and yaw (CamStateCalcDir) */
    /* 0x30 */ f32 dist;           /* Camera.dist */
    /* 0x34 */ f32 pitch;          /* Camera.pitch */
    /* 0x38 */ f32 yaw;            /* Camera.yaw */
    /* 0x3C */ f32 unk3C;
} CamState; /* size 0x40 */

/* CamFollowWork distances (CamFollowSetDistance/GetDistance) */
#define CAM_FOLLOW_DIST_NORMAL 0   /* 800 */
#define CAM_FOLLOW_DIST_BACK   1   /* 1100 */
#define CAM_FOLLOW_DIST_FRONT  2   /* 600 */

/* Follow camera work camFollowWork, reached through camFollow
 * (CamFollowInit 0x005CA598, CamFollowUpdate 0x005CB060). */
typedef struct CamFollowWork {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ f32 speed;         /* approach speed (CamFollowClearTurn) */
    /* 0x00C */ f32 turn;          /* pad turn this frame (radians) */
    /* 0x010 */ f32 heightSpeed;   /* camFollowHeightSpeed is saved here (CamAiFollowUpdate) */
    /* 0x014 */ u8 pad14[0x10];
    /* 0x024 */ f32 pitch;         /* 0.1396 (8 degrees) */
    /* 0x028 */ u8 pad28[8];
    /* 0x030 */ f32 unk30;         /* 0.1396 */
    /* 0x034 */ f32 dist[3];       /* CAM_FOLLOW_DIST_* */
    /* 0x040 */ f32 rate[4];       /* CamFollowSetRate/GetRate; [0] scales the pad turn */
    /* 0x050 */ FVECTOR unk50;     /* look-at (CamFollowPlaceEye) */
    /* 0x060 */ FVECTOR unk60;     /* eye from CamCalcEyePos */
    /* 0x070 */ FVECTOR unk70;
    /* 0x080 */ FVECTOR unk80;     /* eye (CamFollowPlaceEye) */
    /* 0x090 */ FVECTOR unk90;
    /* 0x0A0 */ FVECTOR unkA0;     /* cleared by CamFollowPlaceEye */
    /* 0x0B0 */ u8 padB0[0x20];
    /* 0x0D0 */ CamState cur;      /* copy of the camera */
    /* 0x110 */ CamState save;     /* behind the target (CamAiLadder passes it) */
    /* 0x150 */ u8 pad150[0x4C0];
} CamFollowWork; /* size 0x610 */

extern CamFollowWork *camFollow;   /* = &camFollowWork */
extern CamFollowWork *camFollowCur;
extern CamFollowWork camFollowWork;

/* Smoothed L1/R1 pressures (CamPadTurnInput 0x005CAA40). */
typedef struct CamPadTurn {
    /* 0x0 */ f32 l;
    /* 0x4 */ f32 r;
} CamPadTurn; /* size 0x8 */

extern CamPadTurn camPadTurn;
extern f32 camPadTurnSpeed;        /* 7 */

/* Triangles drawn by CamDebugTriDraw (camDebugTriBuf, reached through
 * camDebugTris). */
typedef struct CamDebugTris {
    /* 0x000 */ s32 count;
    /* 0x004 */ u8 pad4[0xC];
    /* 0x010 */ FVECTOR verts[128][3];
} CamDebugTris; /* size 0x1810 */

extern CamDebugTris *camDebugTris; /* = &camDebugTriBuf */
extern CamDebugTris camDebugTriBuf;

/* Ring of points the camera follows (CamTrail* 0x005CC060-0x005CC2D8). */
#define CAM_TRAIL_LEN 32

typedef struct CamTrail {
    /* 0x000 */ s32 head;
    /* 0x004 */ s32 tail;
    /* 0x008 */ u8 pad8[8];
    /* 0x010 */ FVECTOR points[CAM_TRAIL_LEN];
} CamTrail; /* size 0x210 */

extern CamTrail *camTrail;         /* = &camTrailBuf */
extern CamTrail camTrailBuf;

extern s32 camFollowState;         /* 0x100 clear / 0 blocked, 0x10 chase */
extern s32 camPresetState;         /* 0x100/0x110 */

/* ---------------------------------------------------------------- camera scripts */

/* Script target values in place of a float argument. */
#define CAM_SCR_KEEP      0x7FFFFFFF /* leave the value */
#define CAM_SCR_KEEP2     0x7FFFFFFE /* leave the value (turn commands) */
#define CAM_SCR_PLAYER    0x7FFFFFFD /* the player position (+ camCtl.height) */
#define CAM_SCR_SAVESTART 0x7FFFFFFB /* camScrSaveStart */
#define CAM_SCR_SAVEMAP   0x7FFFFFFA /* camScrSaveMap */

/* camScrState */
#define CAM_SCR_IDLE   0
#define CAM_SCR_START  1
#define CAM_SCR_RUN    2
#define CAM_SCR_PAUSED 3
#define CAM_SCR_END    4

/* Script list from the map data (MDT +0xA4): count, then offsets from the
 * table start. */
typedef struct CamScrTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offs[1];
} CamScrTable;

/* Interpolator (CamScr*InterpStart, CamScrInterpStep 0x005D0D88). */
typedef struct CamScrInterp {
    /* 0x00 */ f32 start;
    /* 0x04 */ f32 end;
    /* 0x08 */ s32 total;          /* frames */
    /* 0x0C */ s32 period;         /* frames between steps */
    /* 0x10 */ s32 mode;           /* 0 linear, 1 ease in-out, 2 ease in, 3 ease out */
    /* 0x14 */ f32 delta;          /* end - start */
    /* 0x18 */ u8 active;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ s32 count;          /* frames done */
    /* 0x20 */ s32 timer;          /* frames to the next step */
    /* 0x24 */ f32 cur;            /* offset from start */
} CamScrInterp; /* size 0x28 */

/* Interpolation command words (0x10031-0x10054); the turn commands
 * (0x10034-0x10036) have the extra direction word. */
typedef struct CamScrInterpCmd {
    /* 0x00 */ s32 op;
    /* 0x04 */ s32 from;           /* float bits, CAM_SCR_KEEP */
    /* 0x08 */ s32 to;             /* float bits, CAM_SCR_* */
    /* 0x0C */ s32 total;
    /* 0x10 */ s32 period;
    /* 0x14 */ s32 mode;
    /* 0x18 */ s32 turnDir;        /* turn: 0 shortest, 1 increasing, 2 decreasing */
} CamScrInterpCmd;

/* Path animation (commands 0x10061/0x10062, CamScrPathStart 0x005D0EF0). */
typedef struct CamScrPath {
    /* 0x00 */ s16 type;           /* 0 short table, 1 long table of mapPathWork */
    /* 0x02 */ s16 idx;
    /* 0x04 */ s32 count;          /* frames */
    /* 0x08 */ f32 x;              /* offset added to the path */
    /* 0x0C */ f32 y;
    /* 0x10 */ f32 z;
    /* 0x14 */ u8 active;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 frame;
} CamScrPath; /* size 0x18 */

extern s32 *camScrPc;
extern s32 camScrFlags;            /* bit 0 abort, bit 1 skip pause */
extern Camera camScrSaveStart;     /* CAM_SCR_SAVESTART */
extern Camera camScrSaveMap;       /* CAM_SCR_SAVEMAP */
extern f32 camScrSaveStartTurnX;   /* = camScrSaveStart.pitch */
extern f32 camScrSaveStartTurnY;   /* = camScrSaveStart.yaw */
extern f32 camScrSaveStartTurnZ;   /* = camScrSaveStart.roll */
extern f32 camScrSaveStartClipF;   /* = camScrSaveStart.clipFar */
extern f32 camScrSaveStartClip14C; /* = camScrSaveStart.clipNear */
extern f32 camScrSaveStartZoom;    /* = camScrSaveStart.zoom */
extern f32 camScrSaveMapTurnX;     /* = camScrSaveMap.pitch */
extern f32 camScrSaveMapTurnY;     /* = camScrSaveMap.yaw */
extern f32 camScrSaveMapTurnZ;     /* = camScrSaveMap.roll */
extern f32 camScrSaveMapLength;    /* = camScrSaveMap.dist */
extern f32 camScrSaveMapClipF;     /* = camScrSaveMap.clipFar */
extern f32 camScrSaveMapClip14C;   /* = camScrSaveMap.clipNear */
extern f32 camScrSaveMapZoom;      /* = camScrSaveMap.zoom */
extern s32 camScrWait;
extern CamScrTable *camScrTable;
extern s32 camScrId;
extern s32 camScrState;            /* CAM_SCR_* */
extern CamScrInterp camScrMoveInterp[3];   /* lookAt x, y, z */
extern CamScrInterp camScrTurnInterp[3];   /* pitch, yaw, roll (degrees) */
extern CamScrInterp camScrZoomInterp;
extern CamScrInterp camScrPosInterp[3];    /* eye x, y, z */
extern CamScrInterp camScrPersInterp;      /* fov */
extern CamScrInterp camScrLengthInterp;    /* dist */
extern CamScrPath camScrMovePath;          /* drives lookAt */
extern CamScrPath camScrPosPath;           /* drives eye */
extern s32 camScrEndScript[];              /* { 0x1FFFF } */

/* ---------------------------------------------------------------- message cursor */

/* The page cursor request (EvtMsgCursor, evtMsgCursor) is in field/event.h. */

/* Message page cursor (TxtMsgCursor* 0x005D1188-0x005D1548), 2 entries
 * in a 0x120-byte FldMemAlloc block. */
typedef struct TxtMsgCursor {
    /* 0x00 */ u8 used;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u16 x;              /* s16 stores in 0x005D11D0 */
    /* 0x04 */ u16 y;
    /* 0x06 */ s16 pad6;
    /* 0x08 */ f32 u;              /* animation phases 0..1 (s32 zero stores in 0x005D11D0) */
    /* 0x0C */ f32 v;
    /* 0x10 */ GphBatchQuad spr;    /* GphBatchDrawSprites sprite */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x3C];
} TxtMsgCursor; /* size 0x90 */

extern TxtMsgCursor *txtMsgCursorWork;

/* ---------------------------------------------------------------- enemy work */

/* ChrEnemyWork (the record) is in field/state.h. */
typedef struct ChrEnemyWorkPool {
    /* 0x000 */ s32 count;
    /* 0x004 */ ChrEnemyWork recs[64];
} ChrEnemyWorkPool; /* size 0x1004 (ChrEnemyWorkPoolInit) */

extern ChrEnemyWorkPool *chrEnemyWorkPool;

/* ---------------------------------------------------------------- map load */

/* Offset list in the map data (character packs MDT +0xF8/+0x174). */
typedef struct MapOffsetList {
    /* 0x0 */ u32 count;
    /* 0x4 */ s32 offs[1];
} MapOffsetList;

extern MapOffsetList *mapCharPackList;
extern void *mapBlock178;          /* MDT +0x178 block (MapDataLoad) */
extern void *mapBlock178Data;      /* mapBlock178 + its word 2 */
extern struct FilePacMotionPack *mapMotionTable; /* motion table from the map data */

/* mapLoadState.state */
#define MAP_LOAD_IDLE  (-1)
#define MAP_LOAD_READ  0
#define MAP_LOAD_SETUP 1
#define MAP_LOAD_START 10
#define MAP_LOAD_EVENT 11

/* Field load state machine (MapLoadUpdate 0x005D2848). The MDT just read
 * stays cached in one of two buffers: the large one at the memory tail
 * (FldMemGetTailBuf) or the small one (FldMemGetSubBuf, up to 0x11FEFF). */
typedef struct MapLoadState {
    /* 0x00 */ s32 state;          /* MAP_LOAD_* */
    /* 0x04 */ s32 sub;            /* sound bank loading step, -1 done */
    /* 0x08 */ s32 which;          /* 0 large, 1 small */
    /* 0x0C */ u16 largeId;        /* map file number, 0 none (s16 in 0x005D16B8) */
    /* 0x0E */ u16 padE;
    /* 0x10 */ u32 largeBuf;
    /* 0x14 */ u32 largeSize;
    /* 0x18 */ u16 smallId;        /* s16 in 0x005D1698 */
    /* 0x1A */ u16 pad1A;
    /* 0x1C */ u32 smallBuf;
    /* 0x20 */ u32 smallSize;
    /* 0x24 */ u8 *data;           /* decompressed MDT (mapData) */
    /* 0x28 */ s32 size;
    /* 0x2C */ f32 time;           /* load time in frame percent */
} MapLoadState; /* size 0x30 */

extern MapLoadState mapLoadState;

/* Map numbers. */
typedef struct MapNo {
    /* 0x0 */ u16 cur;
    /* 0x2 */ u16 entrance;
    /* 0x4 */ u16 file;            /* MDT number including the variant */
    /* 0x6 */ u16 prev;
    /* 0x8 */ u16 unk8;
} MapNo; /* size 0xA */

extern MapNo mapNo;
extern u16 mapFileNo;              /* = mapNo.file */
extern u8 mapVariantNo;            /* save data byte added to variant map numbers */

/* ---------------------------------------------------------------- ripple textures */

/* GphRipple surface (main GphRippleCreate). */
typedef struct MapRippleSurf {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ u32 w;
    /* 0x1C */ u32 h;
} MapRippleSurf;

/* Animated noise textures (MapRippleLoad/Update/Apply 0x005D3450-0x005D3758). */
typedef struct MapRippleWork {
    /* 0x0 */ MapRippleSurf *surf[2];
    /* 0x8 */ u8 kind[2];          /* texture image kind, 1 = random ripples */
    /* 0xA */ u8 slot[2];          /* texture slot */
    /* 0xC */ u16 timer[2];
    /* 0x10 */ u32 count;
} MapRippleWork; /* size 0x14 */

extern MapRippleWork mapRippleWork;

#endif /* FIELD_N15_H */
