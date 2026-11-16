#ifndef FIELD_STATE_H
#define FIELD_STATE_H

#include "common.h"
#include "camera.h"
#include "gph.h"
#include "chr.h"
#include "sys.h"

/*
 * FIELD core state: fldState, the field cameras (camField, camCtl), field
 * sound, fades, screen effects, treasure, map effects, item spots, tenant
 * (NPC/party character) objects and the common model instance pool.
 *
 * Embedded engine types come from gph.h (fades, screen effects, texture
 * packets), chr.h (face animation) and sys.h (font images). Pointers to
 * other engine types (models, motions, particle emitters, ...) keep their
 * incomplete struct tags.
 */

struct GphModel;        /* main model instance (0x90) */
struct GphMotion;       /* main motion */
struct PtclEmitter;     /* FIELD effect object / emitter (0x12D0) */
struct ChrTenantArea;   /* wander area (0x68), chrTenantAreaTable */
struct ChrTrail;        /* position trail (ChrTrail*) */
struct ChrTenantTrack;  /* tenant AI track (field/n14.h) */
struct FldMoveCmd;      /* queued move command (field/n20.h) */
struct FldSpotDef;      /* item spot definition (field/n22.h) */
struct ChrObj;

/* ---------------------------------------------------------------- engine objects */

/* GphFade (fldFades) and GphScreenFx (fldScreenFx) are in gph.h, ChrFaceAnim
 * (ChrObj.faceAnim) in chr.h, TxtFontImage (fldState.msgFontImages) and
 * TxtWinTex in sys.h. */

/* ---------------------------------------------------------------- fldState */

/* FldState.mode */
#define FLD_MODE_PLAY       0
#define FLD_MODE_MENU       1
#define FLD_MODE_PLAY2      2
#define FLD_MODE_TRANSITION 4

typedef struct FldState {
    /* 0x00 */ s32 mode;           /* FLD_MODE_* (u32 in 0x005B0E60) */
    /* 0x04 */ s32 lightCount;     /* 16 */
    /* 0x08 */ s32 lightList;      /* fldLightList, light list buffer (s32 in its only view) */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 unk10;          /* 1 (FldInit) */
    /* 0x14 */ u32 systemSpeed;    /* fldSystemSpeed, "SYSTEM SPEED" */
    /* 0x18 */ f32 motionSmooth;   /* fldMotionSmooth */
    /* 0x1C */ f32 talkRotSpeed;
    /* 0x20 */ f32 runSpeed;
    /* 0x24 */ f32 kamaeSpeed;     /* fldKamaeSpeed */
    /* 0x28 */ f32 encountRange;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;           /* s8 in 0x005D2848 */
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 pendingBattleStart; /* fldPendingBattleStart */
    /* 0x30 */ u8 pad30[0x50];
    /* 0x80 */ TxtFontImage msgFontImages[3]; /* fldMsgFontImages; 0x005C9360 indexes fldState as {0x80 header, image} x 3 */
} FldState; /* size 0x1400 */

/* ---------------------------------------------------------------- camCtl */

/* CamCtl.aiType (names at 0x00666818) */
#define CAM_AI_TYPE0    0
#define CAM_AI_GRANDIA2 1
#define CAM_AI_EYE      2
#define CAM_AI_LADDER   3
#define CAM_AI_FF       4
#define CAM_AI_TOWN     5
#define CAM_AI_FREE     12
#define CAM_AI_NULL     13

typedef struct CamCtl {
    /* 0x000 */ u8 aiType;         /* CAM_AI_* (s8 in 0x005D8970) */
    /* 0x001 */ u8 subType;        /* preset sub type (CamAiPresetUpdate) */
    /* 0x002 */ u8 pad2[3];
    /* 0x005 */ u8 unk5;           /* < 2 blocks KAMAE and extra parts */
    /* 0x006 */ u8 unk6;
    /* 0x007 */ u8 resetBehind;    /* puts the camera back behind the player */
    /* 0x008 */ u8 unk8;           /* frame counter (max 60) */
    /* 0x009 */ u8 prevType;
    /* 0x00A */ u8 unkA;           /* counter (max 10) */
    /* 0x00B */ u8 playerShown;    /* 0 when the camera is closer than 60 */
    /* 0x00C */ f32 unkC;
    /* 0x010 */ f32 height;        /* look-at height above the target */
    /* 0x014 */ f32 unk14;
    /* 0x018 */ struct ChrObj *target; /* player 1 (s32 in 5 views) */
    /* 0x01C */ u8 pad1C[0x14];
    /* 0x030 */ FVECTOR transitFrom; /* CamSetTransitPoint */
    /* 0x040 */ FVECTOR transitMid;
    /* 0x050 */ Camera saved;
} CamCtl; /* size 0x1D0 (CamFieldInit clears 0x1D0; the bss slot, bss_676480.c, is 0x1E0: nothing reads the last 0x10) */

/* Camera configuration of the map (FldCamConfigLoad 0x005F75A8, from the
 * map set; FldCamConfigGet). */
typedef struct FldCamConfig {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 type;           /* -> camCtl.unk5 (0, 1: town AI; 2, 3, 5: type 0) */
    /* 0x0A */ u16 unkA;
    /* 0x0C */ f32 yaw;            /* camField[0].yaw = yaw + pi / 2 */
    /* 0x10 */ f32 ang[2];         /* degrees, wrapped to -180..180 */
    /* 0x18 */ f32 unk18[7];
    /* 0x34 */ s32 unk34[3];
    /* 0x40 */ u8 unk40[5][2];
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ s32 aux;            /* fldCamConfigAux at load time (FldCamConfigSetAux) */
} FldCamConfig; /* size 0x50 */

/* ---------------------------------------------------------------- fldSnd */

typedef struct FldSnd {
    /* 0x00 */ u32 bgmId;
    /* 0x04 */ u32 bgmHandle;      /* s32 in 0x00616EE8 */
    /* 0x08 */ u32 strmId;         /* second stream */
    /* 0x0C */ u32 strmHandle;     /* s32 in 0x00616F88 */
    /* 0x10 */ u16 stepSe[4];      /* footstep SE ids */
    /* 0x18 */ s16 mapSeBanks[4];
    /* 0x20 */ u16 partySeBanks[3];
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ f32 bgmVolume;
    /* 0x2C */ f32 distScale;      /* SndSetDistanceScale, default 6000 */
} FldSnd; /* size 0x30 */

/* ---------------------------------------------------------------- treasure */

typedef struct FldTreasure {
    /* 0x00 */ FVECTOR path[4];    /* fall path points, [3] = position */
    /* 0x40 */ FVECTOR namePos;    /* name text on screen (FldTreasureDrawNames) */
    /* 0x50 */ f32 nameW;
    /* 0x54 */ f32 nameH;
    /* 0x58 */ f32 fallStep;
    /* 0x5C */ f32 t;              /* fall progress 0..1, then name fade 0..3 */
    /* 0x60 */ f32 dist;           /* to the player (FldTreasureFindNearest) */
    /* 0x64 */ u8 pad64[4];
    /* 0x68 */ s32 inst;           /* chrInstPool slot */
    /* 0x6C */ s32 inst2;
    /* 0x70 */ u32 age;            /* drop order, lowest is reused first */
    /* 0x74 */ s32 flagId;         /* taken flag */
    /* 0x78 */ u16 item;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ u8 active;
    /* 0x81 */ u8 instGroup;       /* model kind (chrInstPool group) */
    /* 0x82 */ u8 nameLen;
    /* 0x83 */ u8 nameRow;
    /* 0x84 */ u8 pad84;
    /* 0x85 */ u8 state;           /* 1 fall, 2/3 near player, 4 chest opening */
    /* 0x86 */ u8 unk86;
    /* 0x87 */ u8 pad87[4];
    /* 0x8B */ u8 id;
    /* 0x8C */ u8 pad8C[4];
} FldTreasure; /* size 0x90 */

typedef struct FldTreasureWork {
    /* 0x0000 */ FldTreasure entries[64];
    /* 0x2400 */ s32 dropSlot;
    /* 0x2404 */ u32 count;
    /* 0x2408 */ u32 dropDir;        /* next drop direction 0-7 (x 45 degrees), stepped per item by FldTreasureSpawnDrops 5D4500 */
} FldTreasureWork; /* size 0x240C */

/* ---------------------------------------------------------------- map effects */

typedef struct EffMapWork {
    /* 0x0000 */ struct PtclEmitter *fireList[48];   /* count +0x11B3 (also the list counted at +0x11B1) */
    /* 0x00C0 */ struct PtclEmitter *charaPool[4];   /* EffSpawnChara/AtPlayer; list counted at +0x11B4 */
    /* 0x00D0 */ struct PtclEmitter *spotOpenPool[4];
    /* 0x00E0 */ u8 padE0[0x10];
    /* 0x00F0 */ struct PtclEmitter *pickupPool[4];
    /* 0x0100 */ struct PtclEmitter *pool16[16];     /* EffSpawnCode04 */
    /* 0x0140 */ struct PtclEmitter *pool140[4];     /* [0] EffSpawnUnusedB (s32 there) */
    /* 0x0150 */ struct PtclEmitter *spotPool[4];    /* EffSpawnSpotKind3, EffSpawnCode32 */
    /* 0x0160 */ struct PtclEmitter *steamList[32];  /* count +0x11B9 */
    /* 0x01E0 */ struct PtclEmitter *poolE0[4];
    /* 0x01F0 */ struct PtclEmitter *pool5A[4];      /* EffSpawnCode5A */
    /* 0x0200 */ struct PtclEmitter *moteList[48];   /* count +0x11B5 */
    /* 0x02C0 */ struct PtclEmitter *ringList[44];   /* count +0x11B0 (EffSpawnCode28/46) */
    /* 0x0370 */ u8 pad370[0x10];
    /* 0x0380 */ GsTexPacket texObjs[9];  /* effect textures from GRXSYS (GsTexLoadPacket) */
    /* 0x0EC0 */ u8 padEC0[0x140];
    /* 0x1000 */ u8 drawEnvs[3][0x90];   /* GsTexSetupDraw render states */
    /* 0x11B0 */ u8 ringCount;
    /* 0x11B1 */ u8 driftCount;          /* EffSpawnCode36, appends to fireList */
    /* 0x11B2 */ u8 unk11B2;
    /* 0x11B3 */ u8 fireCount;
    /* 0x11B4 */ u8 charaCount;
    /* 0x11B5 */ u8 moteCount;
    /* 0x11B6 */ u8 unk11B6[3];
    /* 0x11B9 */ u8 steamCount;
    /* 0x11BA */ u8 pad11BA[6];
} EffMapWork; /* size 0x11C0 */
/* The emitter lists are s32 arrays in the EffSpawnCode* files that only
 * store FldMemAlloc results (0x005DF960, 0x005DFA90, 0x005DFB20, ...). */

/* ---------------------------------------------------------------- motion tables */

/* ChrObj.motTable (+0x68), one 0x100-byte table per object (ChrObjArrayAlloc). */
typedef struct ChrMotTable {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 slots[63];      /* GphMotion *, -1 = empty; ChrMotTableSet/Take accept ids up to 63 */
} ChrMotTable; /* size 0x100 */

/* ---------------------------------------------------------------- item spots */

typedef struct FldSpotHdr {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 motionCount;
    /* 0x08 */ u32 count;
    /* 0x0C */ s32 modelPack;      /* offset, relocated to a pointer */
    /* 0x10 */ s32 motionPack;     /* offset, relocated */
    /* 0x14 */ struct FldSpotDef *entries; /* offset, relocated; 0x20-byte entries */
} FldSpotHdr;

typedef struct FldSpot {
    /* 0x00 */ struct GphModel *model;
    /* 0x04 */ struct ChrObj *chara;
    /* 0x08 */ u8 kind;            /* 1 item, 2 hidden monster, 3 trap damage, 4 heal */
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 pad10[4];
} FldSpot; /* size 0x14 */

typedef struct FldSpotWork {
    /* 0x000 */ FldSpotHdr *hdr;
    /* 0x004 */ void *blob;
    /* 0x008 */ FldSpot *list;
    /* 0x00C */ s32 *itemIds;
    /* 0x010 */ ChrMotTable motTable;  /* the spot objects' ChrObj.motTable */
    /* 0x110 */ u32 count;
    /* 0x114 */ u8 order[0x100];   /* shuffle order */
    /* 0x214 */ u32 modelCount;
    /* 0x218 */ struct GphModel *models[16];
} FldSpotWork; /* size 0x258 (FldSpotInit) */

/* ---------------------------------------------------------------- encounters */

/* Map character / monster definition (fldCharDefTable; a tenant's enemy
 * work points at its entry, ChrEnemyWork.charDef). */
typedef struct FldCharDef {
    /* 0x00 */ s32 kind;           /* low 12 bits enemy kind */
    /* 0x04 */ u16 seInterval;     /* frames between ambient SEs ... */
    /* 0x06 */ s16 seIntervalRand; /* ... plus VuRandInt(this) */
    /* 0x08 */ s16 se;             /* ambient SE (AI modes 1 and 3) */
    /* 0x0A */ s16 alertSe;        /* played on switching to AI mode 2 */
    /* 0x0C */ u8 cost;            /* encounter group cost */
    /* 0x0D */ u8 padD[7];
    /* 0x14 */ u8 unk14;           /* FldMonsterSetTracks presets */
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 pad1A[2];
} FldCharDef; /* size 0x1C */

/* Encounter group of a monster tenant (fldEncGroups entries; FldEncGroupSetup),
 * shown by the debug MONSTER TABLE and tenant GROUP pages. */
typedef struct FldEncSlot {
    /* 0x0 */ u16 defNo;           /* fldCharDefTable / fldChrPackTable index (" NO") */
    /* 0x2 */ u16 kind;            /* FldMapSetEnc.kind */
    /* 0x4 */ u8 count;            /* " MAX" */
    /* 0x5 */ u8 level;            /* " LEVEL" */
} FldEncSlot; /* size 0x6 */

typedef struct FldEncGroup {
    /* 0x00 */ u8 cost;            /* "POINT" */
    /* 0x01 */ u8 count;           /* "GROUP MAX" */
    /* 0x02 */ FldEncSlot slots[4];
} FldEncGroup; /* size 0x1A */

typedef struct FldEncGroups {
    /* 0x0 */ s32 count;
    /* 0x4 */ FldEncGroup *groups; /* per monster tenant */
} FldEncGroups; /* size 0x8 */

/*
 * Enemy work of a tenant (ChrObj.sense; ChrEnemyWorkSetup 0x005D15D8 fills
 * it, the AI ops 0x005C1F20.. read it, the debug GROUP page 0x005D8330 shows
 * it). 0x40-byte records from chrEnemyWorkPool (0x006770B8).
 */
typedef struct ChrEnemyWork {
    /* 0x00 */ FldEncGroup *group; /* its encounter group */
    /* 0x04 */ FldCharDef *charDef;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 range;          /* sight range (ChrTenantCanSeeLeader), 0 = blind */
    /* 0x0C */ s16 fov;            /* sight angle in degrees, 0 = blind */
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 flagId;         /* returned by ChrTenantOpHide */
    /* 0x12 */ s16 seTimer;        /* counts down to the next ambient SE */
    /* 0x14 */ u8 pad14[8];
    /* 0x1C */ u16 flags;          /* 1 sense, 4 sight check, 8 waiting (op 2) */
    /* 0x1E */ u8 pad1E[0x22];
} ChrEnemyWork; /* size 0x40 */

/* ---------------------------------------------------------------- characters */

/* ChrObj.flags (+0xC) */
#define CHR_FLAG_DRAWN      0x1
#define CHR_FLAG_ACTIVE     0x2
#define CHR_FLAG_SHADOW     0x8
#define CHR_FLAG_ENCOUNTER  0x10
#define CHR_FLAG_TENANT     0x1000

typedef struct FldHpGauge {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 animT;
    /* 0x08 */ s32 shown;
    /* 0x0C */ s32 cur;
    /* 0x10 */ s32 max;
} FldHpGauge; /* size 0x14 (FldHpGaugeInit) */

typedef struct ChrLook {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
} ChrLook; /* size 0x20 */

/* Field character object: tenants (NPCs, monsters) and party characters. */
typedef struct ChrObj {
    /* 0x000 */ s16 unk0;
    /* 0x002 */ s16 chrId;         /* ChrRegister id (ChrGetById) */
    /* 0x004 */ u16 aiMode;
    /* 0x006 */ u16 collMask;      /* copy of the collision mask; s16 in some views */
    /* 0x008 */ s32 actFlags;      /* 8/0x10 KAMAE, 0x20 hurt, 0x40 examine, 0x80 falling, 0x100 blink, 0x800 body turn, 0x1000 head look */
    /* 0x00C */ s16 flags;         /* CHR_FLAG_*; u16 in most views, but 0x005FA358 only matches with s16 (and vs andi) */
    /* 0x00E */ s16 opTimer;
    /* 0x010 */ s16 id;
    /* 0x012 */ u8 stunAnim;       /* stun phase timer (ChrStunUpdate) */
    /* 0x013 */ u8 stunSpin;       /* stun star ring angle, 0..180 */
    /* 0x014 */ u8 unk14;
    /* 0x015 */ u8 ctrlMode;       /* 0 pad, 1 script, 2 ladder */
    /* 0x016 */ u8 ladderState;
    /* 0x017 */ u8 pad17;
    /* 0x018 */ u8 unk18;
    /* 0x019 */ u8 unk19;
    /* 0x01A */ u8 stunTimer;      /* stun duration (state 2) */
    /* 0x01B */ u8 stunState;      /* 1 appear, 2 hold, 3 vanish */
    /* 0x01C */ u8 pad1C[2];
    /* 0x01E */ s16 unk1E;
    /* 0x020 */ u8 pad20[8];
    /* 0x028 */ s32 unk28;
    /* 0x02C */ u32 blinkTimer;    /* blinks while counting: drawn when % 7 >= 3 */
    /* 0x030 */ f32 speed;         /* s32 in 3 views */
    /* 0x034 */ f32 radius;        /* talk/collision radius */
    /* 0x038 */ f32 unk38;
    /* 0x03C */ f32 leaderDist;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ f32 unk44;
    /* 0x048 */ f32 motSpeed;
    /* 0x04C */ s32 htaArea;
    /* 0x050 */ s32 moveDir;       /* 16-bit angle units */
    /* 0x054 */ s32 heading;       /* 16-bit angle units */
    /* 0x058 */ s32 headingDeg;
    /* 0x05C */ s8 area;
    /* 0x05D */ s8 opState;
    /* 0x05E */ s8 opPc;
    /* 0x05F */ u8 mot;            /* current motion (ChrMotSet) */
    /* 0x060 */ struct GphModel *model;
    /* 0x064 */ struct FldMoveCmd *moveCmds; /* move-command list */
    /* 0x068 */ ChrMotTable *motTable;
    /* 0x06C */ struct ChrTrail *trail;
    /* 0x070 */ ChrEnemyWork *sense; /* tenants: enemy work (sight, SE) */
    /* 0x074 */ ChrFaceAnim faceAnim; /* ChrFaceAnimInit/Set/Update; DbgChrFacePage 0x005D76E8 */
    /* 0x080 */ struct ChrTenantTrack *tracks[8][7]; /* AI tracks per AI mode */
    /* 0x160 */ struct ChrTenantArea *areas[8]; /* wander areas */
    /* 0x180 */ FVECTOR vel;
    /* 0x190 */ FVECTOR unk190;
    /* 0x1A0 */ FVECTOR moveTarget;
    /* 0x1B0 */ u8 pad1B0[0x10];
    /* 0x1C0 */ FVECTOR unk1C0;
    /* 0x1D0 */ FldHpGauge hpGauge; /* party characters */
    /* 0x1E4 */ ChrLook look[2];
    /* 0x224 */ u16 wallHitMax;    /* "BORDER MAX" */
    /* 0x226 */ u8 pad226[2];
    /* 0x228 */ u32 wallHits[0x142]; /* map wall indices hit (0x005C0628 writes up to 400) */
} ChrObj; /* size 0x730 */

typedef struct ChrTenantList {
    /* 0x0 */ u32 count;           /* capacity (ChrTenantAlloc) */
    /* 0x4 */ u32 used;            /* chrTenantUsed */
    /* 0x8 */ s32 unk8;
    /* 0xC */ ChrObj *objs;        /* chrTenantObjs */
} ChrTenantList; /* size 0x10 */

/* ---------------------------------------------------------------- instance pool */

typedef struct ChrInst {
    /* 0x0 */ u8 used;
    /* 0x1 */ u8 visible;
    /* 0x2 */ u8 spin;             /* turns 0x100 per frame */
    /* 0x3 */ u8 blink;            /* draw skips frames with value % 7 < 3 */
    /* 0x4 */ s32 unk4;
    /* 0x8 */ struct GphModel *model;
} ChrInst; /* size 0xC */

typedef struct ChrInstPool {
    /* 0x0000 */ s32 count;
    /* 0x0004 */ ChrInst slots[48][32]; /* [model group][slot] */
} ChrInstPool; /* size 0x4804 */

/* ---------------------------------------------------------------- globals */

extern FldState fldState;
extern TxtWinTex txtMsgWinTexture[3];     /* 0x0067CC00 = &fldMsgFontImages[0].texPacket (0x680 stride) */
extern TxtWinTex txtWinTexturesBase[];    /* 0x0067B880: TxtWinTextureBind 0x005E9F20 indexes it with 3..5 */
extern Camera camField[2];                /* [1] is view 6 */
extern CamCtl camCtl;
extern FldCamConfig fldCamConfig;
extern s32 fldCamConfigAux;
extern FldSnd fldSnd;
extern GphFade fldFades[2];
extern GphScreenFx fldScreenFx[2];
extern GphScreenFx fldScreenFx2;          /* = fldScreenFx[1] */
extern FldTreasure *fldTreasureWork;      /* FldTreasureWork, indexed as entries */
extern EffMapWork *effMapWork;
extern FldSpotWork *fldSpotWork;
extern ChrTenantList chrTenantList;
extern ChrInstPool *chrInstPool;
extern FldEncGroups fldEncGroups;
extern FldCharDef *fldCharDefTable;

#endif /* FIELD_STATE_H */
