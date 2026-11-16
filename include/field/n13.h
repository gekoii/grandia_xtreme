#ifndef FIELD_N13_H
#define FIELD_N13_H

#include "common.h"
#include "vu.h"
#include "gph.h"
#include "field/occ.h"   /* OCC entries and per-kind parameters (were here) */

/*
 * FIELD OCC action handlers (0x005B67E0-0x005C0250), map walls
 * (0x005C0250-0x005C0958), map entrances (0x005C0958-0x005C0DC8) and the
 * field object helpers (0x005C0DC8-0x005C1190).
 */

/* ---------------------------------------------------------------- paths */

/* A map path (fldPathTable, from the map data, 5D1828). */
typedef struct FldPath {
    /* 0x0 */ s16 id;              /* CamScrPathStart looks paths up by id */
    /* 0x2 */ s16 nodeStart;       /* control points in fldPathNodeData (MapPathBuild) */
    /* 0x4 */ s16 nodeCount;
    /* 0x6 */ s16 eventStart;      /* events in fldPathEventData */
    /* 0x8 */ s16 eventCount;
    /* 0xA */ u8 padA[2];
} FldPath; /* size 0xC */

/* FldPathEvent.type */
#define FLD_PATH_EVENT_WAIT      0  /* wait arg frames */
#define FLD_PATH_EVENT_OCC_WAKE  1
#define FLD_PATH_EVENT_OCCS_WAKE 2

typedef struct FldPathEvent {
    /* 0x0 */ s16 frame;
    /* 0x2 */ u8 type;             /* FLD_PATH_EVENT_* */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 arg;
    /* 0x6 */ u8 pad6[2];
} FldPathEvent; /* size 0x8 */

extern FldPath *fldPathTable;
extern s32 fldPathCount;               /* entries in fldPathTable (map set header +0x80) */
extern FldPathEvent *fldPathEventData;

/* ---------------------------------------------------------------- map walls */

/*
 * Wall record (MapWallBuildChains 0x005C0390, MapWallBuildCylinders
 * 0x005C08C0): a chain of points (MapWallAllocChain, 0x20 + count * 0x10
 * bytes) or a cylinder (MapWallAllocCylinder, 0x30 bytes, count 0).
 */
typedef struct MapWall {
    /* 0x00 */ s32 count;          /* chain points, 0 for a cylinder */
    /* 0x04 */ f32 radius;         /* bounding circle in XZ */
    /* 0x08 */ union {
        s32 i;
        u16 h;                     /* height added to the top */
    } attr;                        /* from the source record; != 0 reports the wall index (MapWallCollectInArea) */
    /* 0x0C */ s16 top;
    /* 0x0E */ s16 bottom;
    /* 0x10 */ f32 cx;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 cz;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ FVECTOR points[1];  /* chains only */
} MapWall; /* size 0x20 + count * 0x10 (0x30 for a cylinder) */

typedef struct MapWallList {
    /* 0x0000 */ s32 count;
    /* 0x0004 */ MapWall *items[1024];
} MapWallList; /* size 0x1004 (MapWallInit) */

extern MapWallList *mapWallList;     /* selected layer (MapWallSelectLayer) */
extern MapWallList *mapWallListMove; /* layer 0: character movement */
extern MapWallList *mapWallListCam;  /* layer 1: camera */

/* ---------------------------------------------------------------- map entrances */

/* MapEntrance.effect */
#define MAP_ENTRANCE_FADE     1   /* fade in from black over effectTime */
#define MAP_ENTRANCE_CAPTURE  2   /* captured-frame fade (GphScreenFxSet mode 4) */
/* < 0: fixed 6-frame fade, -1 walk in (speed 20), -2 face dir, -3 walk in (speed 40) */

typedef struct MapEntrance {
    /* 0x00 */ VuVec3 pos;         /* copied as a vector (VuVecCopy) */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 dir[4];         /* walk-in / facing direction */
    /* 0x20 */ f32 yaw;            /* camera yaw, degrees */
    /* 0x24 */ f32 unk24;
    /* 0x28 */ u16 id;
    /* 0x2A */ s16 keepYaw;        /* != 0: camera yaw = mapEntranceKeepYaw */
    /* 0x2C */ s16 effect;         /* MAP_ENTRANCE_* */
    /* 0x2E */ s16 effectTime;
    /* 0x30 */ s16 walkTime;       /* > 0: walk in along dir */
    /* 0x32 */ u8 pad32[0xE];
} MapEntrance; /* size 0x40 */

typedef struct MapEntranceTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ MapEntrance *items;
} MapEntranceTable;

extern MapEntranceTable *mapEntranceTable;
extern s32 mapEntranceCur;
extern f32 mapEntranceKeepYaw;

/* ---------------------------------------------------------------- field objects */

/* Map models registered for lookup by id (FldObjGet, FldObjFindById); the id
 * is the s16 at GphModel +0x4. */
typedef struct FldModelTable {
    /* 0x000 */ u32 count;
    /* 0x004 */ GphModel *items[256];
} FldModelTable; /* size 0x404 */

extern FldModelTable fldModelTable;

#endif /* FIELD_N13_H */
