#ifndef FIELD_N22_H
#define FIELD_N22_H

#include "common.h"
#include "vu.h"
#include "field/state.h"  /* FldCharDef, FldEncGroup, ChrObj */
#include "field/n21.h"   /* AmapWork, AmapPlace, AmapItemPlace, amapFloorIndex */

/*
 * FIELD 0x006132D0-0x0061D210: auto-map (random dungeon) floor finalisation
 * and runtime, field screen overlay images, field sound (BGM/stream helpers,
 * positional SE emitters), point lights, the field heap, the battle
 * transition, objects near the player, face portrait windows, item spots and
 * the monster/encounter group setup.
 */

struct GphModel;

/* ---------------------------------------------------------------- 2D sprites */

/* Sprites of GphBatchDrawSprites lists are gph.h GphBatchQuads (corners 0
 * and 1 carry the texture and screen rectangles; a list ends at type 0). */

/* ---------------------------------------------------------------- map set file */

/* Encounter table entry of the map set file (16 at +0x20). */
typedef struct FldMapSetEnc {
    /* 0x0 */ u16 kind;            /* enemy kind (AmapFindEncounterKind; s16 there) */
    /* 0x2 */ s16 lvMax;           /* level range */
    /* 0x4 */ s16 lvMin;
    /* 0x6 */ s8 chance;           /* cumulative %, 100 ends the table */
    /* 0x7 */ s8 count;            /* enemies per pick, also the cost factor (u8 in 0x0061CF28) */
    /* 0x8 */ s8 order;            /* sort key of the picks */
    /* 0x9 */ s8 defNo;            /* fldCharDefTable index + 1 (0 is set to 1) */
    /* 0xA */ u8 padA[2];
} FldMapSetEnc; /* size 0xC */

/* Generator algorithm choice of the map set file (5 at +0xF4). */
typedef struct FldMapSetAlgo {
    /* 0x0 */ u8 algo;             /* AmapWork.algo1 */
    /* 0x1 */ s8 chance;           /* cumulative % */
    /* 0x2 */ s8 unk2;             /* AmapWork.nodeDensity */
    /* 0x3 */ u8 unk3;             /* AmapWork +0x3D */
    /* 0x4 */ u8 unk4;             /* AmapWork.alcoveCount */
    /* 0x5 */ u8 pad5[3];
} FldMapSetAlgo; /* size 0x8 */

/* Map set file record (MapSetFileGet): encounters, spots, generator setup. */
typedef struct FldMapSet {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 mapId;         /* amapDungeonTable key */
    /* 0x004 */ u8 pad4[0x14];
    /* 0x018 */ u16 enemySkillCount; /* ptyEnemySkillCount (FldBattleStart) */
    /* 0x01A */ u8 unk1A;
    /* 0x01B */ u8 dropArea1;      /* AmapRollItems kind 1 drop area */
    /* 0x01C */ u8 unk1C;
    /* 0x01D */ u8 dropArea0;      /* AmapRollItems kind 0 drop area */
    /* 0x01E */ s8 monsterCount;   /* FldMonsterSpawnAll (u8 in 0x00614628) */
    /* 0x01F */ u8 pad1F;
    /* 0x020 */ FldMapSetEnc enc[16];
    /* 0x0E0 */ u8 padE0[6];
    /* 0x0E6 */ s16 spotRange;     /* FldSpotFindNear: reach + 40 */
    /* 0x0E8 */ s32 trapDamage;    /* trap spots (FldTrapDamageApply) */
    /* 0x0EC */ s8 spotMonsters;   /* spot kinds 2, 4, 3, 1 (FldSpotSetup) */
    /* 0x0ED */ s8 spotHeals;
    /* 0x0EE */ s8 spotTraps;
    /* 0x0EF */ s8 spotItems;
    /* 0x0F0 */ u8 dropArea2;      /* AmapRollItems kind 2 drop area (spot items) */
    /* 0x0F1 */ u8 spotCount;      /* AmapWork +0x104 (spot count) */
    /* 0x0F2 */ u8 unkF2;
    /* 0x0F3 */ u8 padF3;
    /* 0x0F4 */ FldMapSetAlgo algos[5];
    /* 0x11C */ u8 unk11C;
    /* 0x11D */ u8 pad11D[3];
    /* 0x120 */ u16 unk120;
    /* 0x122 */ u8 unk122;
    /* 0x123 */ u8 pad123;
    /* 0x124 */ u8 unk124;
    /* 0x125 */ u8 pad125[3];
    /* 0x128 */ u16 unk128;
    /* 0x12A */ u8 unk12A;
} FldMapSet;

/* ---------------------------------------------------------------- map data records */

/* Positional SE emitter of the map data (FldSndEmitterLoad; AmapBuildSoundEmitters). */
typedef struct FldSndEmitterDef {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 se;
    /* 0x04 */ s8 attachKind;      /* 0 none, 1 field object, 2 tenant */
    /* 0x05 */ s8 attachId;
    /* 0x06 */ s8 shape;           /* 0 point, 1 box, 2 sphere */
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s16 pos[3];         /* box: first corner */
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 pos2[3];        /* box: second corner; sphere: [2] radius */
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u16 unk18[4];
} FldSndEmitterDef; /* size 0x20 */

/* Point light of the map data (FldPointLightLoad; AmapBuildPointLights). */
typedef struct FldPointLightDef {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ s8 attachKind;      /* 0 none, 1 field object, 2 tenant, 3 character */
    /* 0x04 */ u8 attachId;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 enable;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ f32 pos[3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 intensity;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 color[3];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 range;
    /* 0x3C */ f32 unk3C;
} FldPointLightDef; /* size 0x40 */

/* Item spot placement (map data spot table; AmapPlaceSpots). Plain arrays:
 * FVECTOR members (16-byte alignment) change 0x006142E0's code. */
typedef struct FldSpotPlace {
    /* 0x00 */ f32 pos[4];
    /* 0x10 */ f32 scale[4];       /* copied to the model's scale */
    /* 0x20 */ s32 rot[4];         /* copied to the model's rotation ([3] unused) */
} FldSpotPlace; /* size 0x30 */

typedef struct FldSpotPlaceTable {
    /* 0x00 */ u32 count;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ FldSpotPlace spots[1];
} FldSpotPlaceTable;

/* ---------------------------------------------------------------- auto-map */

/* Dungeon table entry (amapDungeonTable, 30). */
typedef struct AmapDungeon {
    /* 0x00 */ u16 mapId;
    /* 0x02 */ u16 lastFloor;      /* compared with the exit object's +0x38 */
    /* 0x04 */ u16 resetFlag;      /* exit resets amapFloorIndex */
    /* 0x06 */ u16 exitMap;        /* AmapGetExitDest (| 0x40000000) */
    /* 0x08 */ u16 firstFloorNo;   /* floor banner number of floor 0 */
    /* 0x0A */ u16 spotChance;     /* % chance of the item spots */
    /* 0x0C */ u16 gridSize;       /* AmapWork +0x3B */
    /* 0x0E */ u16 unkE;           /* AmapWork +0x110 .. +0x118 */
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16;
} AmapDungeon; /* size 0x18 */

/* AmapObjRec (AmapWork.objs) is in field/n21.h. */

/* Treasure record built for the floor (AmapBuildTreasures; AmapWork.treasures). */
typedef struct AmapTreasureRec {
    /* 0x00 */ s16 id;
    /* 0x02 */ s16 kind;           /* 5 or 3 */
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ f32 unk10;          /* 500 */
    /* 0x14 */ f32 rot;            /* degrees */
    /* 0x18 */ u16 item;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
} AmapTreasureRec; /* size 0x20 */

/* Entrance record built for the floor (AmapBuildEntrances; AmapWork.entrances). */
typedef struct AmapEntranceRec {
    /* 0x00 */ s32 id;             /* 0x101 start, 0xFFFF exit */
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ f32 rot;            /* degrees */
    /* 0x14 */ f32 rot2;
    /* 0x18 */ s16 unk18;          /* -2 */
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
} AmapEntranceRec; /* size 0x20 */

/* Auto-map runtime state (amapState, AmapInit). */
typedef struct AmapState {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u8 unk10[0x10];     /* qword copied from scratchpad 0x70002740 */
    /* 0x20 */ u8 pad20[0xC];
    /* 0x2C */ s32 *objPack;       /* object model pack (AmapLoadObjPack) */
    /* 0x30 */ s32 loaded;
    /* 0x34 */ s32 cellModelCount;
    /* 0x38 */ u32 cellInstCount;  /* amapCellInsts */
    /* 0x3C */ u32 objModelCount;  /* amapObjModels */
    /* 0x40 */ s32 objPackCount;   /* objPack[0] */
    /* 0x44 */ s32 texId;          /* GphTexBind id of the cell models (u8 in 0x00616748) */
    /* 0x48 */ u8 bannerOn;        /* floor banner */
    /* 0x49 */ u8 bannerFloor;
    /* 0x4A */ u8 bannerAlpha;
    /* 0x4B */ u8 pad4B;
    /* 0x4C */ f32 bannerT;        /* slide-in 0..1 */
    /* 0x50 */ u8 pad50[0x5C0 - 0x50];
} AmapState; /* size 0x5C0 */

/* Cell model instance (amapCellInsts). */
typedef struct AmapCellInst {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u8 clip;           /* GphMeshClipTest result, 0xFF culled */
    /* 0x003 */ u8 pad3[0x7D];
    /* 0x080 */ u8 mtx[0x40];      /* local -> world */
    /* 0x0C0 */ u8 viewMtx[0x40];  /* mtx * camera */
    /* 0x100 */ u8 pad100[0x40];
} AmapCellInst; /* size 0x140 */

extern AmapWork amapWork;
extern AmapState *amapState;
extern AmapCellInst *amapCellInsts;
extern struct GphModel **amapCellModels;  /* per cell type (32) */
extern struct GphModel **amapObjModels;   /* up to 256 */
extern AmapDungeon amapDungeonTable[30];
extern u8 amapSaveFloorNo;       /* save data: floor banner number */

/* ---------------------------------------------------------------- overlay images */

/* Screen overlay image table (fldOverlayImg, FldOverlayImgInit). */
typedef struct FldOverlayImg {
    /* 0x0000 */ u8 tex[80][0x140];  /* texture packets (GsTexPacketSet) */
    /* 0x6400 */ union {
        u64 all;                   /* the update/draw read count and show/alpha as one dword */
        struct {
            s32 count;
            u16 show;
            s16 alpha;             /* 0..0x7F */
        } s;
    } u;
    /* 0x6408 */ u8 pad6408[8];
    /* 0x6410 */ GphBatchQuad spr;
    /* 0x6450 */ s32 sprEnd;       /* list terminator */
    /* 0x6454 */ u8 pad6454[0x6C];
} FldOverlayImg; /* size 0x64C0 */

extern FldOverlayImg *fldOverlayImg;

/* ---------------------------------------------------------------- sound */

/* FldSndEmitterLoad (0x006178D8) only matches when the box floats are
 * stored through a union: with plain f32 stores gcc keeps the source s16s
 * in registers instead of reloading them. */
typedef union FldSndEmitterF {
    f32 f;
} FldSndEmitterF;

/* Positional SE emitter (fldSndEmitters, 0x1000 bytes = 32). */
typedef struct FldSndEmitter {
    /* 0x00 */ u16 id;
    /* 0x02 */ s16 se;             /* SE id, also the FldSndEmitterEnableById key */
    /* 0x04 */ s16 slot;           /* SE slot, -1 none */
    /* 0x06 */ s16 state;          /* 0 waiting, 1 playing */
    /* 0x08 */ s16 timer;
    /* 0x0A */ s16 shape;          /* FldSndEmitterDef.shape */
    /* 0x0C */ u16 flags;          /* 1 enabled, 4 one shot */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ FVECTOR pos;
    /* 0x20 */ s32 attach;         /* object followed (its +0x10 position), 0 none */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 period;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u16 unk38[4];
    /* 0x40 */ FldSndEmitterF boxMin[3]; /* box corners relative to pos */
    /* 0x4C */ u8 pad4C[4];
    /* 0x50 */ FldSndEmitterF boxMax[3]; /* sphere: [0] radius */
    /* 0x5C */ u8 pad5C[0x24];
} FldSndEmitter; /* size 0x80 */

extern FldSndEmitter *fldSndEmitters;
extern u32 fldSndEmitterCount;
extern s32 fldSndStreamFrames;   /* frames the stream has been playing */
extern u32 fldSndBgmHandle;      /* = fldSnd.bgmHandle */
extern u32 fldSndStreamId;       /* = fldSnd.strmId */
extern u32 fldSndStreamHandle;   /* = fldSnd.strmHandle */

/* ---------------------------------------------------------------- point lights */

/*
 * GphLight as embedded in FldPointLight at +0x1C: the same layout, but
 * gph.h's GphLight is 16-byte aligned (FVECTOR members) and cannot sit
 * there.
 */
typedef struct FldLightRec {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s32 id;
    /* 0x08 */ s32 type;           /* 1 point */
    /* 0x0C */ s32 enable;
    /* 0x10 */ f32 pos[4];
    /* 0x20 */ f32 color[4];
    /* 0x30 */ f32 range;
    /* 0x34 */ f32 intensity;
} FldLightRec; /* size 0x38 */

/* Field point light (fldPointLights, 32). */
typedef struct FldPointLight {
    /* 0x00 */ u16 enable;
    /* 0x02 */ u16 id;
    /* 0x04 */ u16 kind;           /* attach kind 0-3, 4 flicker */
    /* 0x06 */ u16 unk6;           /* FldPointLightDef.unk5: part id */
    /* 0x08 */ s32 attach;         /* object followed */
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u16 flickerTimer;   /* kind 4: frames since the last range change */
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ f32 range;          /* flicker base range */
    /* 0x18 */ f32 unk18;
    /* 0x1C */ FldLightRec light;
    /* 0x54 */ u8 pad54[8];
    /* 0x5C */ s32 unk5C;
} FldPointLight; /* size 0x60 */

typedef struct FldPointLightSortEnt {
    /* 0x0 */ s32 index;
    /* 0x4 */ s32 dist;
} FldPointLightSortEnt; /* size 0x8 */

extern FldPointLight *fldPointLights;
extern u32 fldPointLightCount;
extern FldPointLightSortEnt *fldPointLightSort;

/* ---------------------------------------------------------------- field heap */

extern u32 fldMemTop;            /* FldMemAlloc bump heap */
extern u32 fldMemEnd;
extern u32 fldMemCur;
extern u32 fldMemSubTop;         /* 0x11FF00 area in the BATTLE MAGIC heap */
extern u32 fldMemSubEnd;

/* ---------------------------------------------------------------- encounters */

/* FldCharDef, FldEncSlot, FldEncGroup, FldEncGroups, fldEncGroups and
 * fldCharDefTable are in field/state.h. */

/* ---------------------------------------------------------------- near objects */

/* Animated map object near the player (mapNearObjTable). */
typedef struct MapNearObj {
    /* 0x00 */ FVECTOR pos;        /* rest position of part 1 */
    /* 0x10 */ f32 *partPos;       /* ChrInstGetPos */
    /* 0x14 */ f32 dist;           /* to the player */
    /* 0x18 */ f32 range;
    /* 0x1C */ f32 alpha;
    /* 0x20 */ f32 t;              /* bob 0..1 */
    /* 0x24 */ u8 id;
    /* 0x25 */ u8 state;           /* 0 gone */
    /* 0x26 */ u16 inst;           /* chrInstPool slot */
    /* 0x28 */ u16 group;          /* chrInstPool group */
    /* 0x2A */ u16 fading;
    /* 0x2C */ u8 pad2C[4];
} MapNearObj; /* size 0x30 */

typedef struct MapNearObjTable {
    /* 0x000 */ MapNearObj objs[32];
    /* 0x600 */ u32 count;
} MapNearObjTable; /* size 0x610 */

extern MapNearObjTable *mapNearObjTable;

/* ---------------------------------------------------------------- face windows */

/* Face portrait window (evtFaceWins, 3). */
typedef struct EvtFaceWin {
    /* 0x0000 */ u16 active;
    /* 0x0002 */ u16 id;           /* portrait */
    /* 0x0004 */ s16 x;
    /* 0x0006 */ s16 y;
    /* 0x0008 */ s16 w;
    /* 0x000A */ s16 h;
    /* 0x000C */ s16 nameX;
    /* 0x000E */ s16 nameY;
    /* 0x0010 */ u16 z;
    /* 0x0012 */ u16 nameLen;
    /* 0x0014 */ u16 unk14;          /* = nameLen while the name quads are built */
    /* 0x0016 */ u8 pad16[0xA];
    /* 0x0020 */ GphBatchQuad spr;
    /* 0x0060 */ s32 sprEnd;       /* list terminator */
    /* 0x0064 */ u8 pad64[0x3C];
    /* 0x00A0 */ GphBatchQuad quads[0xB0]; /* name GphBatchDrawQuads list */
} EvtFaceWin; /* size 0x2CA0 */

extern EvtFaceWin *evtFaceWins;

/* ---------------------------------------------------------------- spots */

/* Spot model entry of the spot data (FldSpotHdr.entries; 0x20 each). */
typedef struct FldSpotDef {
    /* 0x00 */ u16 model;
    /* 0x02 */ s16 inst;           /* fldSpotWork->models index, -1 = model */
    /* 0x04 */ s32 motion;         /* open motion (u16 in 0x0061C2B8) */
    /* 0x08 */ s32 motion2;        /* -1 = motion */
    /* 0x0C */ u8 padC[0x14];
} FldSpotDef; /* size 0x20 */

extern u32 *fldSpotModelPack;    /* model pack of the spot models, [0] model count */

#endif /* FIELD_N22_H */
