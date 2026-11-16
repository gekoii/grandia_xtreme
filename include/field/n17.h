#ifndef FIELD_N17_H
#define FIELD_N17_H

#include "common.h"
#include "vu.h"
#include "chr.h"           /* ChrPack, ChrPackBinding */
#include "field/state.h"   /* ChrMotTable */

/*
 * FIELD glue for the particle system, tenant (NPC) setup, character-vs-map
 * collision (borders, HTA areas, polygons), map models, party character
 * objects, the model instance pool passes, sky, shadow and lens flare
 * (0x005E0950-0x005E9F18). Shared types such as ChrObj, ChrInstPool and
 * PtclFldWork live in field/state.h and ptcl.h; this header holds the rest.
 */

struct GphModel;

/* ---------------------------------------------------------------- particles */

/* A FIELD effect's anim set texture (PtclFldRes.animSets[i]->tex) is a gph.h
 * GphTex: PtclFldEffectLoad stores the registered id at +0x60 as a
 * doubleword and PtclFldSystemCreate reads it back. */

/* ---------------------------------------------------------------- character packs */

/* ChrPack (relocated SYS/PARTY.PAC or map MDT pack) and ChrPackBinding
 * (ChrPackBind 0x00453B88; fldPartyPackTable entries, 9 x 0x2C, cleared as
 * 0x18C by 0x0065FF40) are in chr.h. */

/* Map character/monster pack slot: fldChrPackTable (64, ChrTenantInit) and
 * mapExtraCharPacks (5, 0xF0 cleared by MiniInit 0x00645DB0). 5D2848 stores
 * the pack both here at +0x2C and (through ChrPackBind) at bind.pack. */
typedef struct FldChrPack {
    /* 0x00 */ ChrPackBinding bind;
    /* 0x2C */ ChrPack *pack;
} FldChrPack; /* size 0x30 */

extern FldChrPack *fldChrPackTable;        /* 0x006763A8, 64 */
extern FldChrPack mapExtraCharPacks[5];    /* 0x0067E7C0 */
extern ChrPackBinding fldPartyPackTable[9]; /* 0x0067DF20 */

/* ---------------------------------------------------------------- tenant definitions */

/* AI track / wander area ids of one tenant definition slot. */
typedef struct ChrTenantDefSlot {
    /* 0x0 */ s16 area;            /* wander area (ChrTenantAreaGet) */
    /* 0x2 */ s16 tracks[6];       /* AI tracks (ChrTenantTrackGet) */
} ChrTenantDefSlot; /* size 0xE */

/* Tenant definition from the map (0xC0, ChrTenantSetup 0x005E1AA8). */
typedef struct ChrTenantDef {
    /* 0x00 */ u16 id;             /* model id - 1000 */
    /* 0x02 */ s16 mode;           /* 2 -> 1; 3 -> 1 with one random wander area; also shifted into flags (u16 in 5E1978) */
    /* 0x04 */ s16 pack;           /* fldChrPackTable / fldPartyPackTable slot */
    /* 0x06 */ u16 objId;          /* -> ChrObj.id */
    /* 0x08 */ u8 unk8;            /* -> ChrObj.unk19 */
    /* 0x09 */ s8 global;          /* party character pack (slot + 1) */
    /* 0x0A */ u16 flags;          /* ChrTenantSetFlagsFromDef */
    /* 0x0C */ f32 angle;          /* heading in degrees */
    /* 0x10 */ u8 pad10[0x40];
    /* 0x50 */ ChrTenantDefSlot slots[8];
} ChrTenantDef; /* size 0xC0 */

/* ---------------------------------------------------------------- collision */

/* mapCollFlags (0x00677148) bits: what MapCollMove tests ("COLLISION MENU"). */
#define MAP_COLL_CHAR    0x1
#define MAP_COLL_BORDER  0x2
#define MAP_COLL_HTA     0x4
#define MAP_COLL_POLYGON 0x8

struct MapWall;   /* wall record (field/n13.h): count 0 = cylinder/circle */
struct EvtArea;   /* field/event.h */

/* Wall segment of a gathered border (MapBorderAddPolygon/AddSegments). */
typedef struct MapBorderWall {
    /* 0x00 */ FVECTOR *p0;
    /* 0x04 */ FVECTOR *p1;
    /* 0x08 */ f32 dx;             /* unit direction p0 -> p1 */
    /* 0x0C */ f32 dz;
    /* 0x10 */ f32 len;
} MapBorderWall; /* size 0x14 */

typedef struct MapBorderHit {
    /* 0x00 */ s32 type;           /* 0 circle (shape), else wall segment */
    /* 0x04 */ f32 dist;           /* distance to the wall (MapBorderAddPolygon) */
    /* 0x08 */ MapBorderWall wall;
    /* 0x1C */ struct MapWall *shape; /* circle: radius +4, centre +0x10 */
} MapBorderHit; /* size 0x20 */

/* Borders gathered around a position (MapBorderGather, max 64). */
typedef struct MapBorderHitList {
    /* 0x000 */ MapBorderHit hits[64];
    /* 0x800 */ s32 count;
} MapBorderHitList; /* size 0x804 (MapBorderInit) */

/* HTA (event area) collision candidates (MapHtaGatherMove/Query). */
typedef struct MapHtaHitList {
    /* 0x000 */ s32 count;
    /* 0x004 */ struct EvtArea *areas[64];
} MapHtaHitList; /* size 0x104 */

/* Collision triangle (MapPolyAdd copies the first 0x40 bytes). */
typedef struct MapPolyTri {
    /* 0x00 */ FVECTOR v0;
    /* 0x10 */ FVECTOR e1;         /* v1 - v0 */
    /* 0x20 */ FVECTOR e2;         /* v2 - v0 */
    /* 0x30 */ FVECTOR normal;
    /* 0x40 */ s32 kind;           /* MapPolyResolve: 0 floor (|normal.y| >= cos 45), 1 low wall, 2 wall */
    /* 0x44 */ u8 pad44[0xC];
} MapPolyTri; /* size 0x50 */

typedef struct MapPolyList {
    /* 0x0 */ s32 count;           /* max 256 */
    /* 0x4 */ MapPolyTri *tris;    /* 0x5000-byte buffer (MapPolyInit) */
} MapPolyList; /* size 0x8 */

extern s32 mapCollFlags;                  /* 0x00677148, MAP_COLL_* (default 0xFF) */
extern MapBorderHitList *mapBorderHitList; /* 0x0067714C */
extern MapHtaHitList mapHtaHitList;       /* 0x0067C300 */
extern MapPolyList mapPolyList;           /* 0x006762D8 */
extern struct GphModel *mapCollModel;     /* 0x006771C0, layer 0 (movement) */
extern struct GphModel *mapCamCollModel;  /* 0x006771C4, layer 1 (camera) */

/* ---------------------------------------------------------------- map models */

/* Map model entry (MapModelLoadPac 0x005E7B30 / MapModelPlace 0x005E80F8).
 * Models with an id >= 250 sway (GphModelDeformWave in MapModelCalcPass). */
typedef struct MapModel {
    /* 0x000 */ u8 visible;
    /* 0x001 */ u8 waveOn;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ f32 wavePhase;     /* degrees, +1 per frame */
    /* 0x008 */ f32 waveAmp;
    /* 0x00C */ f32 waveSpeed;
    /* 0x010 */ ChrMotTable motTable; /* motion table given to ChrMotBlendUpdate (MapModelUpdate) */
    /* 0x110 */ struct GphModel *model;
} MapModel; /* size 0x114 */

extern MapModel *mapModelTable;   /* 0x006771C8 */
extern s32 mapModelCount;         /* 0x006771CC; u32 in 0x005E7B30 */

/* ---------------------------------------------------------------- party characters */

extern struct GphModel *chrPartyModels[9]; /* 0x006771D0, ChrPartyObjCreate */

/* ---------------------------------------------------------------- sky, shadow, lens flare */

/* FldSky.follow */
#define FLD_SKY_FOLLOW_XYZ 0   /* kept at the camera position */
#define FLD_SKY_FOLLOW_XZ  1   /* x/z only */

typedef struct FldSky {
    /* 0x0 */ s32 enable;
    /* 0x4 */ s32 follow;          /* FLD_SKY_FOLLOW_* */
    /* 0x8 */ struct GphModel *model;
} FldSky; /* size 0xC */

/* Lens flare ghost quads are gph.h GphBatchQuads (GphBatchDrawQuads list). */

extern FldSky fldSkyTable[2];     /* 0x006771F8 */
extern FMATRIX fldShadowMtx;      /* 0x00677240, GphShadowCalcMtx; [3][1] = model y + 4 per object */
extern f32 fldLensFlareAlpha;     /* 0x00677280, intensity (1.0 at init) */
extern FVECTOR fldSunPos;         /* 0x00677290 */
extern u8 fldLensFlareEnable;     /* 0x006772A0 */

#endif /* FIELD_N17_H */
