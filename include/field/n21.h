#ifndef FIELD_N21_H
#define FIELD_N21_H

/*
 * FIELD 0x006018F0-0x006132D0: the nearby-enemy threat gauge and the auto-map
 * (random dungeon) generator - the generator work (amapWork), its cell map
 * and node list, the placement lists and the OCC entries it writes.
 *
 * Layouts were merged from the per-file views by byte offset.
 */

#include "common.h"
#include "vu.h"        /* VuVec3 */
#include "field/occ.h"   /* OccEntry, OccTween, OccSwitch (AmapOccSet*) */

/* ---------------------------------------------------------------- threat gauge */

/*
 * FldThreatGauge work (fldThreatGauge, FldMemAlloc'd by FldThreatGaugeInit):
 * the 19-bar meter comparing the party's average level with the monster
 * groups within 4000 units (FldThreatGaugeUpdate), drawn by 0x006020A8.
 */
typedef struct FldThreatGauge {
    /* 0x000 */ u32 level;           /* bars lit, 0..19 */
    /* 0x004 */ s16 barAlpha[19];    /* 0..128 */
    /* 0x02A */ u8 pad2A[2];
    /* 0x02C */ f32 pulseAngle;      /* degrees, += pulseSpeed */
    /* 0x030 */ s32 alarm;           /* a group 5+ levels above the party */
    /* 0x034 */ u8 pad34[0xC0C];
    /* 0xC40 */ u8 texPacket[0x140]; /* GsTexLoadPacket target (FldThreatGaugeInit) */
    /* 0xD80 */ f32 alarmFade;
    /* 0xD84 */ f32 pulseSpeed;
    /* 0xD88 */ f32 fadeIn;          /* 0..1; reset with 0 stored as s32 in 2 views */
    /* 0xD8C */ u8 vibrateTimer;     /* PadVibrate every 20 frames while alarm */
    /* 0xD8D */ u8 prevFlags;        /* level flags of the last frame (SE on change) */
    /* 0xD8E */ u8 seTimer;          /* alarm SE every 30 frames */
    /* 0xD8F */ u8 padD8F;
    /* 0xD90 */ u16 showDelay;       /* frames before the gauge shows again, max 60 */
    /* 0xD92 */ u8 padD92[0x2E];
} FldThreatGauge; /* size 0xDC0 */

extern FldThreatGauge *fldThreatGauge;

/* ---------------------------------------------------------------- generator work */

/* Record types of field/n22.h that amapWork's buffers hold. */
struct FldMapSet;
struct AmapTreasureRec;
struct FldSndEmitterDef;
struct AmapEntranceRec;
struct FldSpotPlaceTable;
struct FldPointLightDef;

/* Spawned object record (amapWork.objs; AmapGimmicksSpawn writes them,
 * AmapBuildPointLights reads them). */
typedef struct AmapObjRec {
    /* 0x0 */ u8 type;               /* low 5 bits */
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 x;
    /* 0x4 */ s16 y;
    /* 0x6 */ s16 z;
} AmapObjRec; /* size 0x8 */

/* Path of the floor (amapWork.pathRanges, pathCount entries): points
 * start..end of amapWork.pathPoints. */
typedef struct AmapPathRange {
    /* 0x0 */ u16 start;
    /* 0x2 */ u16 end;
    /* 0x4 */ u8 pad4[4];
} AmapPathRange; /* size 0x8 */

/* OCCS record built for the floor (amapWork.occs, occsCount entries;
 * AmapGimmicksSpawn): the map data's OCCS record that OccsLoad (0x005B66D0)
 * converts into an OccsEntry. */
typedef struct AmapOccsRec {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 areaId;
    /* 0x04 */ s16 flags;
    /* 0x06 */ s16 cond[7];          /* OccCond */
    /* 0x14 */ s16 ops[32];          /* OccsCmd.op */
    /* 0x54 */ s16 args[32];         /* OccsCmd.arg */
} AmapOccsRec; /* size 0x94 */

/* Gimmick/monster placement entry (amapWork gimmicks +0x12C, monsters +0x134). */
typedef struct AmapPlace {
    /* 0x0 */ u8 type;               /* low 5 (monsters 6) bits type, bits 6-7 facing; 0x20 flag */
    /* 0x1 */ u8 x;
    /* 0x2 */ u8 flag;               /* AmapGimmicksScatter */
    /* 0x3 */ u8 y;
    /* 0x4 */ u16 arg;               /* AmapGimmicksSpawn */
} AmapPlace; /* size 0x6 */

/* Item placement entry (amapWork items +0x130, AmapAlcovesPlace). */
typedef struct AmapItemPlace {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 facing;             /* bits 6-7 */
    /* 0x2 */ u8 x;
    /* 0x3 */ u8 y;
    /* 0x4 */ u16 unk4;
} AmapItemPlace; /* size 0x6 */

/* Switch or gate of a template group (AmapSwitchFlagsUpdate, AmapGimmicksSpawn). */
typedef struct AmapSwitch {
    /* 0x0 */ u16 id;                /* switch: dungeon flag id - 16 */
    /* 0x2 */ u16 key;               /* switches and gates with the same key belong together */
    /* 0x4 */ u16 area;              /* area index when added */
} AmapSwitch; /* size 0x6 */

/*
 * A stamped template (AmapTemplateStamp): its cell position and the switches
 * and gates it holds. AmapGimmicksSpawn appends gates to groups[0] past [2]
 * (up to 16, into the following groups).
 */
typedef struct AmapTmplGroup {
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ u8 switchCount;
    /* 0x03 */ u8 gateCount;
    /* 0x04 */ AmapSwitch switches[4];
    /* 0x1C */ AmapSwitch gates[2];
    /* 0x28 */ u8 pad28[2];
} AmapTmplGroup; /* size 0x2A */

/* Template candidates for one area (AmapTemplateListsBuild, AmapTemplatePick):
 * two lists, [0].id holding the count. */
typedef struct AmapTmplCand {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 kind;               /* 1, 2: at most one of each per floor (tmplFlags) */
} AmapTmplCand; /* size 0x2 */

typedef struct AmapTmplList {
    /* 0x000 */ AmapTmplCand cands[2][64];
} AmapTmplList; /* size 0x100 */

/* Template pick log (AmapTemplatePick), shown by AmapDbgStatusDraw. */
typedef struct AmapTmplLog {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 area;               /* "group" */
    /* 0x2 */ u8 list;               /* "new" */
    /* 0x3 */ u8 index;              /* "point" */
} AmapTmplLog; /* size 0x4 */

/* Flood-fill stack (amapWork +0x178, AmapDist*). */
typedef struct AmapPtList {
    /* 0x0 */ u32 count;
    /* 0x4 */ u32 max;               /* high-water mark */
    /* 0x8 */ struct {
        s8 x;
        s8 y;
    } pts[1];                        /* up to 0x1000 */
} AmapPtList;

/*
 * Auto-map generator work (amapWork, 0x00677520; AmapGenReset sets it up,
 * AmapGenerate fills it, AmapLoad loads the floor from it). The buffers at
 * +0x120..+0x188 are carved out of one FldMemAlloc'd block by AmapGenerate.
 */
#ifndef AMAP_WORK_DEFINED
#define AMAP_WORK_DEFINED
typedef struct AmapWork {
    /* 0x000 */ struct FldMapSet *mapSet; /* map set file record; AmapGenBuild clears its monsterCount */
    /* 0x004 */ u8 pad4[8];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 algo1;            /* "algorithm1", 1..8 */
    /* 0x00E */ u8 algo2;            /* "algorithm2": the layout builder */
    /* 0x00F */ u8 mapType;          /* "map type" 0..7, from mapNo */
    /* 0x010 */ u8 result;           /* 0 ok, else retry (AmapGenBuild); special room | 0x80 */
    /* 0x011 */ u8 voidFilled;       /* AmapVoidFillAll */
    /* 0x012 */ u8 startFacing;      /* bits 6-7 (AmapStartFacingSet) */
    /* 0x013 */ u8 width;            /* cells */
    /* 0x014 */ u8 height;
    /* 0x015 */ u8 startX;
    /* 0x016 */ u8 startY;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 exitX;
    /* 0x019 */ u8 exitY;
    /* 0x01A */ u8 itemMax;
    /* 0x01B */ u8 monsterMax;
    /* 0x01C */ u8 nodeCount;        /* nodes to scatter (AmapNodesScatter) */
    /* 0x01D */ u8 unk1D;
    /* 0x01E */ u8 groupCount;       /* groups[] used */
    /* 0x01F */ u8 pad1F;
    /* 0x020 */ u8 alcoveCount;
    /* 0x021 */ u8 typeFlag;         /* amapTypeFlagTable[mapType * 2] */
    /* 0x022 */ u8 unk22;
    /* 0x023 */ u8 unk23;            /* treasures [0, unk23) are kind 5 (FldMapSet.unk1C) */
    /* 0x024 */ u8 dropArea1;        /* FldMapSet.dropArea1 */
    /* 0x025 */ u8 pad25;
    /* 0x026 */ s16 trapArg1[3];     /* AmapOccSetTrap arguments by trap kind */
    /* 0x02C */ s16 trapArg2[3];
    /* 0x032 */ u8 unk32;            /* count of gimmick 9 */
    /* 0x033 */ u8 unk33;            /* count of gimmick 0xA */
    /* 0x034 */ u8 unk34;
    /* 0x035 */ u8 pad35;
    /* 0x036 */ u16 mapNo;           /* map number of the floor (0xA0xx, 0xD8xx) */
    /* 0x038 */ u16 floor;           /* amapFloorIndex after AmapFloorAdvance */
    /* 0x03A */ u8 algoPick;         /* FldMapSetAlgo.algo of the pick (also algo1) */
    /* 0x03B */ u8 gridSize;         /* AmapDungeon.gridSize: width and height */
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 unk3D;            /* FldMapSetAlgo.unk3 (also unk1D) */
    /* 0x03E */ u8 unk3E;            /* FldMapSet.unkF2; unk3E + unk3F = dead-end monsters (AmapDeadEndMonstersPlace) */
    /* 0x03F */ u8 unk3F;
    /* 0x040 */ u16 tileCounts[64];  /* AmapTileCountsBuild, [63] = 0xFF */
    /* 0x0C0 */ s32 bufSize;         /* AmapGenGetBufSize */
    /* 0x0C4 */ s32 cellBufSize;     /* AmapGenGetCellBufSize */
    /* 0x0C8 */ s32 genCount;
    /* 0x0CC */ u8 padCC[4];
    /* 0x0D0 */ f32 startPosX;       /* from the first entrance */
    /* 0x0D4 */ f32 startPosY;
    /* 0x0D8 */ f32 startPosZ;
    /* 0x0DC */ u16 areaCount;
    /* 0x0DE */ u16 occCount;
    /* 0x0E0 */ u16 occsCount;
    /* 0x0E2 */ u8 padE2[6];
    /* 0x0E8 */ u16 treasureCount;   /* AmapBuildTreasures */
    /* 0x0EA */ u16 objCount;        /* "gimmick" (spawned) */
    /* 0x0EC */ u16 gimmickCount;
    /* 0x0EE */ u16 itemCount;       /* "item" */
    /* 0x0F0 */ u16 monsterCount;    /* "monster" */
    /* 0x0F2 */ u8 padF2[2];
    /* 0x0F4 */ u16 pathCount;       /* AmapPathsHaveRepeat */
    /* 0x0F6 */ s16 unkF6;
    /* 0x0F8 */ s16 unkF8;
    /* 0x0FA */ s16 unkFA;
    /* 0x0FC */ u8 padFC[4];
    /* 0x100 */ u16 sndEmitterCount; /* AmapBuildSoundEmitters */
    /* 0x102 */ s16 entranceCount;   /* AmapBuildEntrances (2) */
    /* 0x104 */ u16 spotCount;       /* item spots (FldMapSet.spotCount, AmapPlaceSpots) */
    /* 0x106 */ u16 lightCount;      /* point lights (AmapBuildPointLights, max 16) */
    /* 0x108 */ u8 exitFixed;        /* exit at exitFixedX/Y (start placed far from it) */
    /* 0x109 */ u8 exitFixedX;
    /* 0x10A */ u8 exitFixedY;
    /* 0x10B */ u8 startFixed;
    /* 0x10C */ u8 startFixedX;
    /* 0x10D */ u8 startFixedY;
    /* 0x10E */ u16 nodeDensity;     /* 0 w*h/40, 1 /30, 2 /20 */
    /* 0x110 */ u16 unk110;          /* AmapDungeon.unkE */
    /* 0x112 */ u16 unk112;          /* AmapDungeon.unk10 */
    /* 0x114 */ u16 voidGrowRate;    /* percent (AmapVoidGrow) */
    /* 0x116 */ u16 unk116;          /* AmapVoidCarveWallSide, AmapObstacleGimmicksAdd */
    /* 0x118 */ u16 voidFillMode;    /* AmapVoidFillEnclosed */
    /* 0x11A */ u8 specialRooms;     /* "sroom" */
    /* 0x11B */ u8 pad11B;
    /* 0x11C */ u16 tmplFlags;       /* template kinds 1/2 used */
    /* 0x11E */ u8 pad11E[2];
    /* 0x120 */ u8 *tiles;           /* width * height: type (6 bits) | rotation << 6 */
    /* 0x124 */ void *unk124;
    /* 0x128 */ u8 *cells;           /* the 7-layer cell map (fldMapCells) */
    /* 0x12C */ AmapPlace *gimmicks;
    /* 0x130 */ AmapItemPlace *items;
    /* 0x134 */ AmapPlace *monsters;
    /* 0x138 */ AmapObjRec *objs;    /* spawned gimmick objects (objCount) */
    /* 0x13C */ void *areas;
    /* 0x140 */ struct OccEntry *occ;
    /* 0x144 */ void *unk144;
    /* 0x148 */ void *unk148;
    /* 0x14C */ void *unk14C;
    /* 0x150 */ AmapPathRange *pathRanges; /* pathCount */
    /* 0x154 */ VuVec3 *pathPoints;
    /* 0x158 */ void *unk158;
    /* 0x15C */ void *unk15C;
    /* 0x160 */ void *unk160;
    /* 0x164 */ void *unk164;
    /* 0x168 */ struct AmapTreasureRec *treasures;    /* treasureCount */
    /* 0x16C */ struct FldSndEmitterDef *sndEmitters; /* sndEmitterCount */
    /* 0x170 */ struct AmapEntranceRec *entrances;    /* AmapGenerate reads the start position at +4 */
    /* 0x174 */ struct FldSpotPlaceTable *spots;
    /* 0x178 */ AmapPtList *ptList;
    /* 0x17C */ void *unk17C;
    /* 0x180 */ AmapOccsRec *occs;   /* occsCount (AmapGimmicksSpawn) */
    /* 0x184 */ struct FldPointLightDef *lights;      /* lightCount */
    /* 0x188 */ void *unk188;
    /* 0x18C */ u8 unk18C[2];        /* cleared with groups (0x1F8 bytes from here) */
    /* 0x18E */ AmapTmplGroup groups[11];
    /* 0x35C */ u8 pad35C[0x28];
    /* 0x384 */ s32 unk384;
    /* 0x388 */ AmapTmplList tmplLists[10];
    /* 0xD88 */ u8 tmplLogCount;
    /* 0xD89 */ AmapTmplLog tmplLog[16];
    /* 0xDC9 */ u8 padDC9[3];
} AmapWork; /* size 0xDCC */
#endif

/* ---------------------------------------------------------------- cell map */

/* Corridor node (amapNodes, amapNodeCount entries), linked by corridors. */
typedef struct AmapNode {
    /* 0x0 */ u8 x;
    /* 0x1 */ u8 y;
    /* 0x2 */ u8 group;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 floorMask;
} AmapNode; /* size 0x6 */

extern AmapNode *amapNodes;
extern s32 amapNodeCount;

/*
 * The 7-layer cell map: layer l of cell (x, y) is
 * fldMapCells[fldMapLayerSize * l + y * fldMapWidth + x].
 */
extern u8 *fldMapCells;
extern s32 fldMapWidth;
extern s32 fldMapHeight;
extern s32 fldMapLayerSize;

/* Placement bounds (AmapBoundsSet). */
extern s32 amapBoundX0;
extern s32 amapBoundY0;
extern s32 amapBoundX1;
extern s32 amapBoundY1;

extern u32 amapFloorIndex;
extern s8 amapForceAlgo1;            /* -1, else overrides algo1 */
extern s8 amapForceAlgo2;
extern s8 amapForceTemplates[];      /* [0] index, then template ids (-1 none) */
extern u8 amapCornerTileTable[];     /* tile by 2x2 corner mask (AmapTilesBuild) */

#endif /* FIELD_N21_H */
