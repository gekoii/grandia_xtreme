#ifndef FIELD_N12_H
#define FIELD_N12_H

#include "common.h"
#include "field/occ.h"   /* OccHandler, the per-kind parameters */

/*
 * FIELD overlay entry and per-frame callbacks (0x005B0000-0x005B1958), event
 * areas (0x005B2208-0x005B3FB8) and the OCC / OCCS triggers
 * (0x005B3FB8-0x005B67E0). The tables themselves are in field/event.h
 * (EvtArea) and field/occ.h (OccEntry, OccsEntry, the per-kind parameters);
 * this header has what goes around them.
 */

/* ---------------------------------------------------------------- frame */

/* GsGetFrameTimePercent samples of the field frame (debug "SYNC" page
 * 0x005DC0D8 shows the differences): [0]-[9] FldUpdatePlay /
 * FldUpdateNoControl, [9]-[15] FldBuildDrawLists. */
extern f32 fldPerfTimes[16];

/*
 * Draw list handles (GphDrawListSet). Each list is a {size, handle} pair of
 * words; the *Size symbols are the pair, the others its handle word.
 */
extern s32 fldPartyLitDrawList;     /* fixed-light party pass (0x200) */
extern s32 fldTenantDrawList;       /* NPCs (0x200) */
extern s32 fldMapDrawListSize[2];   /* {0x400, fldMapDrawList} */
extern s32 fldMapDrawList;          /* map objects */
extern s32 fldPartyDrawList;        /* party members (0x100) */
extern s32 fldInstDrawListSize[2];  /* {0x100, fldInstDrawList} */
extern s32 fldInstDrawList;         /* model instances (treasure, items) */
extern s32 fldSkyDrawList;          /* sky dome (0x20) */
extern s32 fldPtclDrawList;         /* field particles */
extern s32 fldDrawListMiniMapBuf;   /* mini-map */
extern struct GphPrimList *fldPacketList; /* deferred GS packets (GphPrimListSelect/Draw) */

/* Vertex of the debug wireframes (FldDebugDrawBox, FldDebugDrawCylinder) for
 * GphLine3DDraw: four floats in stack arrays, 4-byte aligned (not FVECTOR). */
typedef struct FldDbgVtx {
    f32 x, y, z, w;
} FldDbgVtx;

/* ---------------------------------------------------------------- event areas */

extern s32 evtAreaUse;              /* "HTA USE": EvtAreaUpdate does nothing while 0 */
extern s32 evtAreaFrameWorkA;       /* zeroed every EvtAreaUpdate, never read */
extern s32 evtAreaFrameWorkB;
extern s32 evtAreaStepPriority;     /* highest start of a hit footstep area (target 3) this frame */
extern s32 evtAreaStepSound;        /* footstep sound kind (its count) */

#endif /* FIELD_N12_H */
