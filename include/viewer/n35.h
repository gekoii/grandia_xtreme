#ifndef VIEWER_N35_H
#define VIEWER_N35_H

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "camera.h"
#include "chr.h"     /* DbgMotObjParam, dbgMotObjParams, ChrMotionList, chrMotionList */

/*
 * VIEWER overlay (0x00700000-0x007027D8): the model/motion viewer program
 * (VwrInit/Load/Update/BuildDrawLists/Draw/EndFrame), its loaders and the
 * dead flock simulator (0x00701988-0x00701C00). The main-executable
 * globals the viewer shares with the debug MOTION page (dbgMotObjParams,
 * chrMotionList) are in chr.h.
 */

/* ---------------------------------------------------------------- viewer */

/* Viewer work (vwrWork, cleared as 0x4C bytes by VwrInit). */
typedef struct VwrWork {
    /* 0x00 */ s32 mainListSize;    /* 0x1000 */
    /* 0x04 */ s32 shadowListSize;  /* 0x1000 */
    /* 0x08 */ u8 pad8[0x18];
    /* 0x20 */ s32 mainList;        /* draw list handle (GphDrawListSet) */
    /* 0x24 */ s32 shadowList;      /* planar shadow pass */
    /* 0x28 */ u8 pad28[0x18];
    /* 0x40 */ s32 lightCount;      /* 4 */
    /* 0x44 */ s32 lightList;       /* GphLightListSet */
    /* 0x48 */ s32 unk48;
} VwrWork; /* size 0x4C */

/* Model instances from the model packs: MODEL.PAC first, then XYZ.PAC
 * (axes) and STAGE.PAC (the stage is last). */
typedef struct VwrModelList {
    /* 0x000 */ u32 count;
    /* 0x004 */ GphModel *items[256];
} VwrModelList; /* size 0x404 */

extern FMATRIX vwrShadowMtx;        /* planar shadow projection (GphShadowCalcMtx) */
extern s32 vwrStatFileBytes;        /* PtclDbgSetExtSizes FILE column */
extern s32 vwrStatInstBytes;        /* INST column */
extern s32 vwrStatSortBytes;        /* SORT column */
extern VwrWork vwrWork;
extern s32 vwrAnimDebugOn;          /* enables VwrAnimDebugDraw (never set) */
extern GphAnimState vwrAnim;        /* stepped by VwrUpdate, drawn by VwrAnimDebugDraw */
extern u8 vwrAnimTex[0x140];        /* texture of the VwrAnimDebugDraw sprite */
extern u8 vwrModelPool[0x14];       /* ChrModelPoolInit header over a 1MB buffer */
extern VwrModelList vwrModelList;
extern Camera vwrCamera;

/* ---------------------------------------------------------------- flock (dead code) */

/* Qword-aligned float vector of the flock code: VwrFlockSetWaypoint copies
 * one with lq/sq, and VwrFlockUpdate's waypoint loads only schedule as the
 * original's with a 16-byte-aligned element type (VuVec4 is 4-aligned). */
typedef struct VwrVec4 {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} __attribute__((aligned(16))) VwrVec4; /* size 0x10 */

/* Flock agent (vwrFlockAgents, 64; VwrFlockInit 0x007019A8, VwrFlockUpdate
 * 0x00701C00). Positions and angles are copied to the models from
 * vwrModelList.items[vwrFlockCfg.firstModel]. */
typedef struct VwrFlockAgent {
    /* 0x000 */ VwrVec4 pos;
    /* 0x010 */ f32 pitch;
    /* 0x014 */ f32 heading;    /* yaw, radians */
    /* 0x018 */ f32 roll;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 dist[128];  /* distance to agent j (vwrFlockDist) */
    /* 0x220 */ f32 speed;
    /* 0x224 */ f32 maxSpeed;
    /* 0x228 */ f32 minSpeed;
    /* 0x22C */ f32 accel;
    /* 0x230 */ f32 climb;      /* vertical speed, +-1.5 */
    /* 0x234 */ f32 climbAccel;
    /* 0x238 */ f32 sepRadius;  /* separation radius */
    /* 0x23C */ s32 avoidTimer; /* 16 after a separation turn */
    /* 0x240 */ s16 unk240;     /* 30 when crossing y = 620 */
    /* 0x242 */ u8 climbed;     /* changed height this frame */
    /* 0x243 */ u8 avoiding;    /* another agent inside sepRadius */
    /* 0x244 */ u8 pad244[2];
    /* 0x246 */ u8 below;       /* y >= 620 */
    /* 0x247 */ u8 slow;        /* speed < 4 */
    /* 0x248 */ u8 pad248[8];
} VwrFlockAgent; /* size 0x250 */

/* Flock settings (vwrFlockCfg, VwrFlockInit). */
typedef struct VwrFlockCfg {
    /* 0x0 */ u16 firstModel;   /* vwrModelList index of agent 0 */
    /* 0x2 */ u16 count;        /* agents */
    /* 0x4 */ u16 waypointCount;
    /* 0x6 */ s16 on;
} VwrFlockCfg; /* size 0x8 */

typedef struct VwrFlockWaypoints {
    /* 0x000 */ VwrVec4 v[64];
    /* 0x400 */ s32 cur;
} VwrFlockWaypoints; /* size 0x410 */

extern VwrFlockAgent vwrFlockAgents[64];
extern VwrFlockCfg vwrFlockCfg;
extern VwrFlockWaypoints vwrFlockWaypoints;

#endif /* VIEWER_N35_H */
