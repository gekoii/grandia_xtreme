#ifndef PTCL_H
#define PTCL_H

/* Particle system of the main executable (PTB controllers, generators,
 * effectors, starter slots, the 31-particle emitter objects) and its FIELD
 * glue (ptclFldWork, sprite slots, dust). Merged from the local views listed
 * per struct; "X in 4xxxxx" notes a file whose view used another type. */

#include "common.h"
#include "gph.h"

/* ------------------------------------------------------------------------ */
/* Effector: 0x1C-byte record in the system's effector pool, copied from the
 * PTB (4019F8, 5E0C50) and edited on the "Effector [%d]" page (40B018).
 * Views: 44ACD8, 44AD28, 44AD78, 44A550,
 * 40B018 (Entry), 4019F8/40B618 (Item), 5E0C50 (Rec). */
typedef struct PtclEffector {
    /* 0x00 */ s8 id;          /* -1 = free */
    /* 0x01 */ u8 flags;       /* 1 MODE vector (else position), 2 OFS on, 4 SYS local (else world) */
    /* 0x02 */ s16 cycle;      /* CYCLE */
    /* 0x04 */ f32 scale;      /* SCL */
    /* 0x08 */ f32 scaleRange; /* SCL random width */
    /* 0x0C */ s32 angX;       /* ANG X (20-bit angle) */
    /* 0x10 */ s32 angXRange;
    /* 0x14 */ s32 angY;
    /* 0x18 */ s32 angYRange;
} PtclEffector; /* size = 0x1C */

/* ------------------------------------------------------------------------ */
/* Generator definition: 0x150 bytes, four of them (keys) per controller record
 * and a live copy at PtclGen+0x60. Field names from the live generator debug
 * pages (407A10 POS, 408420 EMIT, 408F38 SCALE, 409C98 COLOR, 40A578 ROT).
 * Fully typed view: 44DB20 Key (also 44B040 Key, 44C600 Param, 4501A0 Sub,
 * 4019F8/40B618 Sub); 44A748, 44BBF0 and 44D110 use this type.
 * Aligned 8: 44A748 copies it as u64s (struct copy ld/sd), 40B618 needed
 * aligned(8) on the controller record. */
typedef struct PtclGenDef {
    /* 0x000 */ f32 pos[3];          /* POS X/Y/Z; 4501A0 views pos as FVECTOR */
    /* 0x00C */ s32 unk0C;
    /* 0x010 */ f32 color[4];        /* AMB R/G/B/A; 4501A0 views it as FVECTOR */
    /* 0x020 */ s32 rot[3];          /* ROT X/Y/Z */
    /* 0x02C */ s8 effectors[4];     /* EFFE A-D, effector pool indices; 44DB20/44B040 view them as u8 */
    /* 0x030 */ s32 emitAngX;        /* AX (used when gen flag 4) */
    /* 0x034 */ s32 emitAngXRange;   /* AX width */
    /* 0x038 */ s32 emitAngY;        /* AY (used when gen flag 8) */
    /* 0x03C */ s32 emitAngYRange;
    /* 0x040 */ f32 speed;           /* SPD */
    /* 0x044 */ f32 speedRange;
    /* 0x048 */ f32 accel;           /* ACC (gen flag 0x10) */
    /* 0x04C */ f32 accelRange;
    /* 0x050 */ s16 life;            /* LIFE */
    /* 0x052 */ s16 lifeRange;
    /* 0x054 */ f32 rate;            /* PROB - emission per frame (PtclGenIsAlive tests rate + rateRange) */
    /* 0x058 */ f32 rateRange;
    /* 0x05C */ s32 angX;            /* ANG X - spawn position angles */
    /* 0x060 */ s32 angXRange;
    /* 0x064 */ s32 angY;
    /* 0x068 */ s32 angYRange;
    /* 0x06C */ f32 radius;          /* RADIUS */
    /* 0x070 */ f32 radiusRange;
    /* 0x074 */ s16 onceNum;         /* ONCE NUM */
    /* 0x076 */ s8 sxCurve[3];       /* SX easing ST-C1, C1-C2, C2-ED; 44DB20/44B040/44C600 view u8 */
    /* 0x079 */ s8 syCurve[3];       /* SY easing; 44DB20/44B040 view u8 */
    /* 0x07C */ s16 sxC1;            /* SX key times C1, C2 */
    /* 0x07E */ s16 sxC2;
    /* 0x080 */ s16 syC1;
    /* 0x082 */ s16 syC2;
    /* 0x084 */ f32 sx[10];          /* SX0, RATE ST, C1, C2, ED - (value, random) pairs */
    /* 0x0AC */ f32 sy[10];          /* SY0 ... */
    /* 0x0D4 */ s16 sinXLen;         /* SIN X L (period, <= 0 off) */
    /* 0x0D6 */ s16 sinXStart;       /* STD (phase) */
    /* 0x0D8 */ f32 sinXRate;        /* RATE (amplitude) */
    /* 0x0DC */ s16 sinYLen;
    /* 0x0DE */ s16 sinYStart;
    /* 0x0E0 */ f32 sinYRate;
    /* 0x0E4 */ s8 colorKeys[16];    /* ST, C1, C2, ED RGBA, stored as value - 0x80 */
    /* 0x0F4 */ s16 colorC1;         /* colour key times */
    /* 0x0F6 */ s16 colorC2;
    /* 0x0F8 */ s8 colorCurve[3];    /* 44DB20 views u8 */
    /* 0x0FB */ s8 rdxMode;          /* RDX MD (rotation speed mode); 44DB20 views u8 */
    /* 0x0FC */ s8 rdyMode;
    /* 0x0FD */ s8 rdzMode;
    /* 0x0FE */ u8 padFE[2];
    /* 0x100 */ s32 rot0[6];         /* RX0, RY0, RZ0 - (angle, random) pairs */
    /* 0x118 */ s32 rdx[4];          /* RDX start, start random, end, end random */
    /* 0x128 */ s32 rdy[4];
    /* 0x138 */ s32 rdz[4];
    /* 0x148 */ u8 pad148[8];
} __attribute__((aligned(8))) PtclGenDef; /* size = 0x150 */

/* ------------------------------------------------------------------------ */
/* Controller record (PTB): 0x230-byte header of key tracks + 4 generator
 * definition keys. A track interpolates its PtclGenDef fields between the
 * keys; a step switches byte fields at its key times.
 * Views: 44DB20 Model (tracks), 44F550, 4019F8/40B618 Rec/Big,
 * 44AF90 (raw offsets), 5E0C50 Ent (FIELD). */
typedef struct PtclTrack {
    /* 0x0 */ s8 lastKey;   /* 0 = off, else index of the last key used (time[lastKey]) */
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 curve[3]; /* easing per segment */
    /* 0x8 */ s16 time[4];  /* key times */
} PtclTrack; /* size = 0x10 */

typedef struct PtclStep {
    /* 0x0 */ s8 on;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 time[3]; /* switch to keys 1, 2, 3 at these times */
} PtclStep; /* size = 0x8 */

typedef struct PtclCtrl {
    /* 0x000 */ s32 groupCount;   /* particle groups (31 particles each) of the generator */
    /* 0x004 */ s32 flags;        /* 1 colour packets */
    /* 0x008 */ s32 genFlags;     /* initial PtclGen.flags (cleared by PtclCtrlTracksClear) */
    /* 0x00C */ u8 blendMode;     /* initial PtclGen.blendMode (byte load in 44A748); the debug
                                   * page 403568 edits it as an s32 (cast there) */
    /* 0x00D */ u8 pad0D[3];
    /* 0x010 */ s32 length;       /* timeline length in frames */
    /* 0x014 */ s32 resIndex;     /* PTB resource (string) index: texture/model/anim set */
    /* 0x018 */ PtclStep effectorStep;  /* effectors */
    /* 0x020 */ PtclTrack posX;
    /* 0x030 */ PtclTrack posY;
    /* 0x040 */ PtclTrack posZ;
    /* 0x050 */ PtclTrack rotX;
    /* 0x060 */ PtclTrack rotY;
    /* 0x070 */ PtclTrack rotZ;
    /* 0x080 */ PtclTrack color;
    /* 0x090 */ PtclTrack emitAngX;
    /* 0x0A0 */ PtclTrack emitAngY;
    /* 0x0B0 */ PtclTrack speed;
    /* 0x0C0 */ PtclTrack accel;
    /* 0x0D0 */ PtclTrack life;
    /* 0x0E0 */ PtclTrack rate;
    /* 0x0F0 */ PtclTrack angX;
    /* 0x100 */ PtclTrack angY;
    /* 0x110 */ PtclTrack radius;
    /* 0x120 */ PtclTrack onceNum;
    /* 0x130 */ PtclTrack sinX;
    /* 0x140 */ PtclStep sxCurveStep;
    /* 0x148 */ PtclTrack sxTime;
    /* 0x158 */ PtclTrack sx0;
    /* 0x168 */ PtclTrack sxRate;
    /* 0x178 */ PtclTrack sinY;
    /* 0x188 */ PtclStep syCurveStep;
    /* 0x190 */ PtclTrack syTime;
    /* 0x1A0 */ PtclTrack sy0;
    /* 0x1B0 */ PtclTrack syRate;
    /* 0x1C0 */ PtclStep colorCurveStep;
    /* 0x1C8 */ PtclTrack colorTime;
    /* 0x1D8 */ PtclTrack colorKeys;
    /* 0x1E8 */ PtclTrack rotXAnim;
    /* 0x1F8 */ PtclStep rdxModeStep;
    /* 0x200 */ PtclTrack rotYAnim;
    /* 0x210 */ PtclStep rdyModeStep;
    /* 0x218 */ PtclTrack rotZAnim;
    /* 0x228 */ PtclStep rdzModeStep;
    /* 0x230 */ PtclGenDef keys[4];
} PtclCtrl; /* size = 0x770 */

/* ------------------------------------------------------------------------ */
/* Generator instance parts (laid out by PtclGenCreate 44A748). */

/* Sprite particle, 31 per group. Views: 44B040 Part. */
typedef struct PtclSprite {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ f32 sizeX;
    /* 0x14 */ f32 sizeY;
    /* 0x18 */ f32 baseX;
    /* 0x1C */ f32 baseY;
    /* 0x20 */ FVECTOR vel;
    /* 0x30 */ FVECTOR acc;
} PtclSprite; /* size = 0x40 */

/* Per-sprite key state. Views: 44B040 PState. */
typedef struct PtclSpriteKey {
    /* 0x00 */ f32 kx[4];
    /* 0x10 */ f32 ky[4];
    /* 0x20 */ s32 life;
    /* 0x24 */ s32 time;
    /* 0x28 */ u8 pad28[8];
} PtclSpriteKey; /* size = 0x30 */

/* 2D anim stepping state of one sprite (44BBF0 AnimState, 44B040 Tex): the
 * gph.h anim state. */
typedef GphAnimState PtclAnimState;

/* Model particle. Views: 44C600 Particle, 44FB38 Elem. */
typedef struct PtclModelPart {
    /* 0x00 */ s32 life;
    /* 0x04 */ s32 time;
    /* 0x08 */ s32 rotFrom[3];
    /* 0x14 */ s32 rotStep[3];
    /* 0x20 */ f32 scale;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ FVECTOR vel;
    /* 0x40 */ FVECTOR acc;
    /* 0x50 */ f32 size[4];
} PtclModelPart; /* size = 0x60 */

/* Generator instance. Views: 44AC60, 44ADC0, 44B040/44C600 Emitter, 44DA50,
 * 44DB20, 44FB38, 4501A0,
 * debug pages 407A10/408420/408F38/409C98/40A578 Unit; FIELD 5E12F0 Pose. */
typedef struct PtclGen {
    /* 0x00 */ s16 index;        /* slot in PtclSystem.gens */
    /* 0x02 */ s16 liveCount;    /* live particles, -1 = released */
    /* 0x04 */ s32 flags;        /* 1 DIR GENE, 2 FOLLOW, 4 USE AX, 8 USE AY, 0x10 USE ACC, 0x20 ACC STOP,
                                    0x80 SIZE X=Y, 0x100 RGB CHANGE, 0x200 ADD MODE, 0x400 stopped,
                                    0x800 R XYZ, 0x1000/0x2000 RH on/random, 0x4000/0x8000 RV on/random */
    /* 0x08 */ s32 frame;
    /* 0x0C */ s32 emitCount;
    /* 0x10 */ f32 emitAccum;    /* emission accumulator */
    /* 0x14 */ s16 groupCount;
    /* 0x16 */ s8 blendMode;     /* ALPHA - blend/fog state (PtclDrawSprites) */
    /* 0x17 */ u8 texId;         /* TEX ID, also groups generators in the list (44A748 "key") */
    /* 0x18 */ PtclSprite *sprites;     /* groupCount * 31; 44B040 views PtclSprite (*)[31], 4501A0 s32 */
    /* 0x1C */ u32 *colorPkt;           /* per group 0x200 bytes (VIF header 0x6E7C0100 + colours); 44B040 u8 (*)[32][4][4], 4501A0 s32 */
    /* 0x20 */ u32 *uvPkt;              /* per group 0x200 bytes (header 0x757C0080 + UVs); 44B040/4501A0 s32 */
    /* 0x24 */ PtclAnimState *anims;
    /* 0x28 */ PtclSpriteKey *keys;     /* 44B040 PtclSpriteKey (*)[31] */
    /* 0x2C */ s32 *groupLive;          /* live count per group */
    /* 0x30 */ s32 animSet;             /* 2D anim set (pointer) from the request */
    /* 0x34 */ struct ChrModelPool *modelPool; /* model pool control block (0x14 bytes) for model particles */
    /* 0x38 */ GphModel **models; /* model particles (non-NULL = model generator); views: s32 *ids
                                            (44FB38), s32 (44DA50, 4501A0, 44B040),
                                            model ** (44C600) */
    /* 0x3C */ PtclModelPart *modelParts;
    /* 0x40 */ s32 rot[3];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ FVECTOR pos;
    /* 0x60 */ PtclGenDef def;
} PtclGen; /* size = 0x1B0 */

/* Request a generator is created from (PtclGenCalcSize/PtclGenCreate);
 * FIELD keeps one per controller as its "link" (5E0C50 Link, 0x10 allocated).
 * Views: 4023A8 Req, 5E0C50 Link. */
typedef struct PtclGenRequest {
    /* 0x0 */ PtclCtrl *ctrl;
    /* 0x4 */ s32 animSet;   /* 2D anim set pointer (type 3 resources); 5E0C50 views a pointer */
    /* 0x8 */ void *model;   /* model source (type 2 resources), NULL = sprites (4023A8 only
                              * matches with a pointer) */
    /* 0xC */ u8 texId;
    /* 0xD */ u8 partPrio;   /* ChrModelSetPartPrio for model particles (FIELD 10) */
    /* 0xE */ u8 padE[2];
} PtclGenRequest; /* size = 0x10 */

/* Starter slot: a controller instance on the system's timeline.
 * Views: 44AF40 Slot, 44DB20 Anim, 44F550 Anim, 44A550 C, 4023A8 Slot,
 * 402300/402590 (ptclStarterInst), FIELD 5E10A0 Slot, 5E12F0 Inst. */
typedef struct PtclStarterSlot {
    /* 0x0 */ PtclCtrl *ctrl;  /* NULL = free; 44AF40/4023A8 view s32 */
    /* 0x4 */ PtclGen *gen;    /* 44AF40/4023A8/5E10A0 view s32 */
    /* 0x8 */ s32 index;
    /* 0xC */ s32 time;
} PtclStarterSlot; /* size = 0x10 */

/* Particle system (PtclSystemCalcSize/Init): header then the three arrays,
 * each 16-byte aligned. Views: 44A550 Pool, 44DA50/44FB38/
 * 4501A0 Mgr, 44AC60 G, 44ACD8 T, 44AD28/44AD78 Table, 44AF40 Mgr,
 * 44DB20 AnimMgr, debug pages Party/
 * Table/ItemTbl (ptclMainPool). */
typedef struct PtclSystem {
    /* 0x00 */ s32 genCount;
    /* 0x04 */ s32 effectorCount;
    /* 0x08 */ s32 slotCount;
    /* 0x0C */ PtclGen **gens;           /* 44A550 views s32 */
    /* 0x10 */ PtclEffector *effectors;  /* 44A550 views s32 */
    /* 0x14 */ PtclStarterSlot *slots;   /* 44A550 views s32 */
} PtclSystem; /* size = 0x18 */

/* ------------------------------------------------------------------------ */
/* DATA/PARTICLE.PTB (and FIELD effect resources' particle data): counts and
 * offsets from the header. Views: 4019F8 Hdr, 40B618 Hdr/Info, 401F20,
 * FIELD 5E0C50/5E10A0 Tbl, 5E12F0 Track. */
typedef struct PtclPtbHeader {
    /* 0x00 */ s32 ctrlCount;
    /* 0x04 */ s32 ctrlOffset;      /* PtclCtrl[ctrlCount] */
    /* 0x08 */ s32 effectorCount;
    /* 0x0C */ s32 effectorOffset;  /* PtclEffector[effectorCount] */
    /* 0x10 */ s32 starterCount;
    /* 0x14 */ s32 starterOffset;   /* PtclStarterEntry[starterCount] */
    /* 0x18 */ s32 stringCount;
    /* 0x1C */ s32 stringOffset;
} PtclPtbHeader; /* size = 0x20 */

/* Starter timeline entry (ptclStarterTable, PTB starters, FIELD effect tracks).
 * Views: 4019F8 Ent, 402590/4028D8 Entry, 40B618 Rec, FIELD 5E12F0 Key, 5E10A0 Ent. */
typedef struct PtclStarterEntry {
    /* 0x00 */ s16 time;    /* start frame, -1 = unused */
    /* 0x02 */ s16 ctrl;    /* controller record index (FIELD: link/slot index) */
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ s32 rot[3];
    /* 0x1C */ s16 id;      /* ptclStartMaskId compares it, -1 = none; 5E12F0 views s32 pad */
    /* 0x1E */ u8 pad1E[2];
} PtclStarterEntry; /* size = 0x20 */

/* ------------------------------------------------------------------------ */
/* Emitter object (PtclEmitter* 0x004506E8-0x00451A20, FIELD presets 6591E0..
 * 65C4A8 and dust 65C000..65C3F0). 31 particles drawn once per point.
 * Views: 4506E8, 451008, 451500, 451A20 Obj, 450FB0 (raw offsets),
 * FIELD 6591E0 Emit (and the other PtclPreset* files), 65C000 Cam. */
typedef struct PtclEmitterParticle {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 rot;
    /* 0x10 */ f32 vel[3];
    /* 0x1C */ f32 rotVel;
    /* 0x20 */ f32 sizeStepX;
    /* 0x24 */ f32 sizeStepY;
    /* 0x28 */ f32 alpha;
    /* 0x2C */ f32 alphaStep;
    /* 0x30 */ f32 life;
    /* 0x34 */ f32 delay;
    /* 0x38 */ f32 lifeMax;
    /* 0x3C */ u8 pad3C[3];
    /* 0x3F */ u8 active;
} PtclEmitterParticle; /* size = 0x40 */

/* Per-particle VU draw block (4 qwords; w of every qword set to 1.0 by 450FB0). */
typedef struct PtclEmitterVert {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ FVECTOR size;
    /* 0x20 */ u8 pad20[0x20];
} PtclEmitterVert; /* size = 0x40 */

typedef struct PtclEmitter {
    /* 0x0000 */ PtclEmitterParticle parts[31];
    /* 0x07C0 */ FVECTOR origin;       /* 451008 "base", 6591E0 "pos" */
    /* 0x07D0 */ FVECTOR dir;
    /* 0x07E0 */ FVECTOR spread;       /* w: motion modes 3/4 life factor */
    /* 0x07F0 */ FVECTOR sizeStart;
    /* 0x0800 */ FVECTOR sizeEnd;
    /* 0x0810 */ FVECTOR accel;
    /* 0x0820 */ FVECTOR colorStart;
    /* 0x0830 */ FVECTOR colorEnd;
    /* 0x0840 */ FVECTOR colorCur;
    /* 0x0850 */ FVECTOR colorStep;
    /* 0x0860 */ FVECTOR unk860;
    /* 0x0870 */ f32 sizeRandX;
    /* 0x0874 */ f32 sizeRandY;
    /* 0x0878 */ f32 unk878;
    /* 0x087C */ f32 unk87C;
    /* 0x0880 */ u8 pad880[0x10];
    /* 0x0890 */ PtclEmitterVert verts[32]; /* 31 used; 4506E8/451500 view 31 + pad */
    /* 0x1090 */ u32 colorTag[4];          /* VIF UNPACK V4-8 header (ptclEmitterColorHeader) */
    /* 0x10A0 */ u8 colors[31][4][4];      /* per particle 4 corner RGBA */
    /* 0x1290 */ f32 spawnRadius;
    /* 0x1294 */ f32 life;
    /* 0x1298 */ f32 interval;
    /* 0x129C */ u8 pad129C[8];
    /* 0x12A4 */ f32 spawnTime;
    /* 0x12A8 */ f32 unk12A8;
    /* 0x12AC */ FMATRIX *points;          /* emission points; FIELD views s32 / void * */
    /* 0x12B0 */ u8 pointCount;            /* FIELD presets view s8 for 0x12B0-0x12BF */
    /* 0x12B1 */ u8 count;                 /* particles (<= 31) */
    /* 0x12B2 */ s8 unk12B2;
    /* 0x12B3 */ u8 fadeFrames;            /* alpha mode 2 */
    /* 0x12B4 */ u8 sizeRandMode;
    /* 0x12B5 */ u8 dirMode;
    /* 0x12B6 */ u8 spawnFlags;            /* 1 flat (y = 0) */
    /* 0x12B7 */ u8 alphaMode;
    /* 0x12B8 */ u8 motionMode;
    /* 0x12B9 */ u8 unk12B9;
    /* 0x12BA */ u8 active;
    /* 0x12BB */ u8 unk12BB;
    /* 0x12BC */ u8 dirAxis[3];            /* dir mode 2: axes that keep dir's sign */
    /* 0x12BF */ u8 pad12BF;
    /* 0x12C0 */ u8 unk12C0[0xC];          /* FIELD dust points its matrix here (65C000) */
    /* 0x12CC */ void *unk12CC;
} PtclEmitter; /* size = 0x12D0 */

/* FIELD dust emitter work ptclDustWork (0x1500, PtclDust*). Views: 65C000,
 * 65C198, 65C2B0, 65C3F0 Cam (which also view +0x890 as mtx[32][4][4] and
 * +0x1090 as u8 col[32][4][4]). */
typedef struct PtclDustPuff {
    /* 0x0 */ f32 size;
    /* 0x4 */ f32 alpha;
    /* 0x8 */ s32 active;
    /* 0xC */ s32 timer;
} PtclDustPuff; /* size = 0x10 */

typedef struct PtclDustWork {
    /* 0x0000 */ PtclEmitter emitter;
    /* 0x12D0 */ u8 pad12D0[0x30];       /* the emitter's point matrix spills here from +0x12C0 */
    /* 0x1300 */ PtclDustPuff puffs[31];
    /* 0x14F0 */ s32 drawn;
    /* 0x14F4 */ u32 next;
    /* 0x14F8 */ u32 mode;
    /* 0x14FC */ u8 pad14FC[4];
} PtclDustWork; /* size = 0x1500 */

/* ------------------------------------------------------------------------ */
/* FIELD effect resource (relocated by PtclFldEffectLoad 5E0988). Views:
 * 5E0988/5E0C50 Res, 5E10A0 Res, 5E12F0 Res. */
typedef struct PtclFldRes {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 modelPac;        /* offset -> model pack (FilePacModelRelocate) */
    /* 0x0C */ s32 motion;          /* offset -> chunk, +0x10 after relocation, given to the models (+0x80) */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ PtclPtbHeader *ptb;  /* particle data (controllers, effectors, starter track) */
    /* 0x18 */ s32 textures[5];     /* texture files (GphTexDataRelocate/Register) */
    /* 0x2C */ void *animSets[5];   /* 2D anim sets (GphAnimSetRelocate), texture at +0xC */
} PtclFldRes; /* size = 0x40 */

/* FIELD particle work ptclFldWork (0xB80, PtclFldInit): 16 effect slots.
 * Views: 5E0950, 5E0988, 5E0C50, 5E0F48, 5E1080, 5E10A0, 5E1258, 5E12F0
 * (5E0988 views +0x24 as s8[0x50][8], +0x2A4 as [0x20][8], +0x6B8 as [0x120];
 *  5E0C50 views +0x6B8 as [0x20]). */
typedef struct PtclFldWork {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u8 stop[16];               /* stop request per slot (PtclFldEffectStop) */
    /* 0x014 */ u8 active[16];
    /* 0x024 */ s8 texIds[16][8];          /* registered texture ids (5 used); 5E0C50 views u8 */
    /* 0x0A4 */ u32 frame[16][8];          /* 5E0F48 views s32 */
    /* 0x2A4 */ GphModel *models[16][8]; /* effect models; 5E12F0 views s32 */
    /* 0x4A4 */ u8 pad4A4[0xC];
    /* 0x4B0 */ FVECTOR offset[16];
    /* 0x5B0 */ IVECTOR rot[16];           /* 16-bit angles from degrees */
    /* 0x6B0 */ u16 spinA;                 /* counter-rotating part angles (PtclFldModelSpinParts) */
    /* 0x6B2 */ u16 spinB;
    /* 0x6B4 */ PtclSystem *system;        /* 5E0C50 views void *, 5E12F0 s32 */
    /* 0x6B8 */ void *modelPacs[16];       /* FilePacModelRelocate results */
    /* 0x6F8 */ u8 pad6F8[0x40];
    /* 0x738 */ PtclGenRequest *links[16][8]; /* 5E12F0 views s32 */
    /* 0x938 */ PtclStarterSlot *slots[16][8];
    /* 0xB38 */ PtclFldRes *res[16];
    /* 0xB78 */ u8 padB78[8];
} PtclFldWork; /* size = 0xB80 */

/* FIELD sprite effect slot (FldSprite* 0x6585C0-0x658D78), ptclFldSlots -> 64.
 * Views: 6588C0, 6589B8, 6589F0, 658BB8, 658D78 Slot. */
typedef struct PtclFldSprite {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 tex;
    /* 0x02 */ u8 pad2[0xE];
    /* 0x10 */ FVECTOR pos;
    /* 0x20 */ s16 vx;
    /* 0x22 */ s16 vy;
    /* 0x24 */ s16 vz;
    /* 0x26 */ u8 unk26;
    /* 0x27 */ u8 size;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 alpha;
    /* 0x2A */ u8 kind;
    /* 0x2B */ u8 flags;
    /* 0x2C */ u8 used;
    /* 0x2D */ u8 repeat;
    /* 0x2E */ u8 pad2E[2];
} PtclFldSprite; /* size = 0x30 */

/* ------------------------------------------------------------------------ */
/* Main executable */
extern s32 ptclStarterFrame;              /* 0x00499D80 */
extern s16 ptclStartMaskId;               /* 0x00499D84 */
extern f32 ptclStartOffsetY;              /* 0x00499D88 */
extern s32 ptclDbgExtFileSize;            /* 0x00499D8C */
extern s32 ptclDbgExtSortSize;
extern s32 ptclDbgExtInstSize;
extern PtclSystem *ptclMainPool;          /* 0x004A97C8 */
extern u8 ptclTexIdTable[8];              /* 0x004A97D0, per PTB resource */
extern void *ptclAnimSetTable[8];         /* 0x004A97D8 */
extern void *ptclModelTable[8];           /* 0x004A97F8, +4 sort entries */
extern PtclStarterEntry ptclStarterTable[16]; /* 0x004A9818 */
extern PtclStarterSlot *ptclStarterInst[16];  /* 0x004A9A18 */
extern s32 ptclCurWork;                   /* 0x004A9A58 */
extern s32 ptclPeakWork;                  /* 0x004A9A5C */
extern s32 ptclStarterWorkSize[16];       /* 0x004A9A60 */
extern s32 ptclSortAllocSize;             /* 0x004A9AA0 */
extern s32 ptclFileSize;                  /* 0x004A9AA4 */
extern PtclPtbHeader *ptclPtbData;        /* 0x004A9AA8 */
extern PtclSystem *ptclCurSystem;         /* 0x004DA880 */
extern FMATRIX ptclWorkMatrix;            /* 0x004DA980 */
extern GphGaObject *ptclEmitterDrawCtx;   /* 0x004DB410, scratchpad 0x70003F80 */
extern GphGaObject ptclEmitterDrawTemplate; /* 0x004DB420, copied to it before each draw */
extern s32 ptclEmitterPause;              /* 0x004DB4A0 */

/* FIELD */
extern PtclFldWork *ptclFldWork;          /* 0x006762D4 */
extern PtclFldSprite *ptclFldSlots;       /* 0x00679D0C, 64 */
extern PtclDustWork *ptclDustWork;        /* 0x00679D14 */

#endif
