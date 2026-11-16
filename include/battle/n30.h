#ifndef BATTLE_N30_H
#define BATTLE_N30_H

#include "common.h"
#include "vu.h"
#include "battle/unit.h"
#include "gph.h"
#include "sys.h" /* SprRect, SprRectListEntry */

/*
 * BATTLE 0x006A0430-0x006ADD10: the per-unit attached effects (Fx*: cast
 * aura, status icons, fail label, wall, regen, anti-magic, stun, bind,
 * defeat burst, combo link, weapon aura), the battle start sequence, the IP
 * gauge and party status panels (Ip*), unit motion (BtlChr*) and the battle
 * particle bursts (PtclBurst*, PtclBtlSpr*). The units are in battle/unit.h,
 * PtclBurst/PtclBurstType in battle/misc.h; this header has the state shared
 * by several of these files.
 */

/* ------------------------------------------------------------------ */
/* BATTLE sprite particles (PtclBtlSpr*, 0x006AC7A0-0x006ADD10)        */
/* ------------------------------------------------------------------ */

/*
 * Sprite particle, 0x40: the sprite buffers of the particle bursts
 * (PtclBurst.parts / parts2) and of the weapon aura (FxWeaponAura.sprites).
 * PtclBtlSprReset clears .life with a 0x40 stride.
 */
typedef struct PtclBtlSpr {
    /* 0x00 */ s8 life;            /* frames left (PtclBtlSprUpdate) */
    /* 0x01 */ s8 lifeExtra;       /* added to the key times (PtclBtlSprDrawKeyed) */
    /* 0x02 */ u8 flags;           /* 1 flip X, 2 flip Y */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ f32 vel[3];         /* added to pos each frame */
    /* 0x10 */ f32 accel[3];       /* added to vel each frame */
    /* 0x1C */ GphAnimState anim;  /* stepped by GphAnimStep */
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ f32 pos[3];         /* PtclBtlSprSetSpeed scales it as the direction */
    /* 0x3C */ f32 scale;
} PtclBtlSpr; /* size 0x40 */

/* Scale keys of PtclBtlSprDrawKeyed (ptclSparkKeys, ptclSparkGlowKeys). */
typedef struct PtclSparkKeys {
    /* 0x00 */ f32 scale[4];
    /* 0x10 */ s8 life;            /* key 0 time */
    /* 0x11 */ s8 lifeRand;        /* max random extra life: PtclBtlSpr.lifeExtra = rand * lifeRand (6ABFD0) */
    /* 0x12 */ s8 t1;              /* key 1, 2 times */
    /* 0x13 */ s8 t2;
} PtclSparkKeys; /* size 0x14 (0x18 between the two tables) */

extern PtclSparkKeys ptclSparkKeys;           /* 0x006DF3A8 */
extern PtclSparkKeys ptclSparkGlowKeys;       /* 0x006DF3C0 */
extern u8 ptclSparkColBlue[4][4];             /* 0x006DF3D8 colour keys (RGBA) for burst types 13-16 */
extern u8 ptclSparkColRainbow[4][4];          /* 0x006DF3E8 types 17-20 */
extern u8 ptclSparkColGreen[4][4];            /* 0x006DF3F8 types 21-24 */
extern u8 ptclSparkGlowColBlue[4][4];         /* 0x006DF408 */
extern u8 ptclSparkGlowColRainbow[4][4];      /* 0x006DF418 */
extern u8 ptclSparkGlowColGreen[4][4];        /* 0x006DF428 */

/* Debris piece of burst type 10 (PtclBurstUpdateDebris), 8 in PtclBurst.parts. */
typedef struct PtclDebris {
    /* 0x00 */ f32 life;
    /* 0x04 */ GphModel *model;    /* s32 in 6ABF70 */
    /* 0x08 */ f32 vel[3];
    /* 0x14 */ s32 rotVel[3];      /* added to model->rot */
} PtclDebris; /* size 0x20 */

extern s32 ptclDebrisModel;        /* 0x006F2CD0 archive entry 1 (debris model pack) */

/*
 * GRXSYS anim sets bound to textures 0x38-0x3D (PtclBurstLoad). A burst
 * type's animSet (PtclBurstType +0xC) points at one of these variables
 * (PtclBtlSprUpdate/Draw dereference it before GphAnimSetSelect).
 */
extern GphAnimSet *ptclAnimSetTex38; /* 0x006E3F08 */
extern GphAnimSet *ptclAnimSetTex39; /* 0x006E3F44 */
extern GphAnimSet *ptclAnimSetTex3A; /* 0x006E3EA4 */
extern GphAnimSet *ptclAnimSetTex3B; /* 0x006E3EE8 */
extern GphAnimSet *ptclAnimSetTex3C; /* 0x006E3EA8 */
extern GphAnimSet *ptclAnimSetTex3D; /* 0x006E3F14 */

/* Cell as GphAnimGetCellUv returns it (a GphAnimCell): its UV corners, which
 * FxBindDraw and PtclBtlSprDraw turn into a SprRect (/ 8 + 1). */
typedef struct PtclCellUv {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s16 u0;
    /* 0x6 */ s16 v0;
    /* 0x8 */ u8 pad8[4];
    /* 0xC */ s16 u1;
    /* 0xE */ s16 v1;
} PtclCellUv; /* size 0x10 */

/* Generic round glow sprite rect (u8[] in 5 files before the switch). */
extern SprRect ptclGlowRect;       /* 0x006DFB30 */

/* ------------------------------------------------------------------ */
/* Per-unit effects (Fx*, 0x006A0430-0x006A5AE8)                       */
/* ------------------------------------------------------------------ */

/* Cast aura (FxCastAuraSet / Update / Register), 16 x 0x20. */
typedef struct FxCastAura {
    /* 0x00 */ s8 kind;            /* 0 off, 1 move/skill (fxCastAuraModelMove), else magic */
    /* 0x01 */ s8 release;         /* 1 fade out, 2 burst (fxCastAuraBurstMotion) */
    /* 0x02 */ s16 state;          /* 0 create, 1 fade in, 2 fade out, 3 burst, 4 free */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 stamp;          /* btlFrameCount when the state was entered */
    /* 0x0C */ f32 u;              /* UV scroll */
    /* 0x10 */ f32 v;
    /* 0x14 */ f32 alpha;
    /* 0x18 */ s32 pool;           /* model instance pool (BtlModelCreateWithPool, MemFree) */
    /* 0x1C */ GphModel *model;    /* views: Eff *, Obj * */
} FxCastAura; /* size 0x20 */

extern FxCastAura fxCastAura[16];  /* 0x006EA080 */
extern s32 fxCastAuraModelMove;    /* 0x006F2214 archive entry 3 */
extern s32 fxCastAuraModelMagic;   /* 0x006F2218 archive entry 4 */
extern GphMotion *fxCastAuraBurstMotion; /* 0x006F221C GRXSYS +0xF4; s32 in 6A0430 */

/* Cast spark (FxCastSparkSpawn / Update / Draw), 50 x 0x30. */
typedef struct FxCastSpark {
    /* 0x00 */ s8 life;
    /* 0x01 */ s8 age;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 alpha;
    /* 0x08 */ VuVec3 vel;
    /* 0x14 */ VuVec3 accel;
    /* 0x20 */ VuVec3 pos;
    /* 0x2C */ f32 scale;
} FxCastSpark; /* size 0x30 */

extern FxCastSpark fxCastSparks[50]; /* 0x006EA280 */
extern s8 fxCastSparkCount;          /* 0x006F2210 */

/* Status icon over a unit (and on the party panels, IpPanel.icons):
 * FxStatusIconSetActive / Move, IpPanelStatusIconSetActive / Move. */
typedef struct FxStatusIcon {
    /* 0x0 */ s32 state;           /* > 0 shown, < 0 fading out, 0 off */
    /* 0x4 */ f32 alpha;
    /* 0x8 */ f32 pos;             /* y offset (units) / x offset (panels), eased to its slot */
    /* 0xC */ f32 scale;           /* grows while fading out */
} FxStatusIcon; /* size 0x10 */

extern FxStatusIcon fxStatusIcons[16][12]; /* 0x006EABE0, one per status bit 0x10..0x8000 */
extern f32 fxStatusIconAlpha[16];  /* 0x006F21D0 (always 1.0) */

/* "Fail" label over a unit (FxFailLabelShow / Update / Draw), 16 x 0xC. */
typedef struct FxFailLabel {
    /* 0x0 */ s16 timer;           /* -1 off */
    /* 0x2 */ u8 frame;            /* fxFailLabelRects index; 6A1B40 reads it as (s8) */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 vy;              /* s32 in 6A1B40, 6A1C30 (only cleared there) */
} FxFailLabel; /* size 0xC */

extern FxFailLabel fxFailLabel[16]; /* 0x006EB7E0 */

/* Wall barrier (FxWallStart / Hit / Update / Register), 16 x 0x18. */
typedef struct FxWall {
    /* 0x00 */ s32 state;          /* 1 create, 2 shown, 3 free */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 flash;          /* s32 in 6A1D00 (only cleared there) */
    /* 0x0C */ s32 stamp;          /* btlFrameCount */
    /* 0x10 */ s32 pool;           /* void * in 6A1DD0, 6A1E10 */
    /* 0x14 */ GphModel *model;    /* void * in 6A1E10 */
} FxWall; /* size 0x18 */

extern FxWall fxWall[16];          /* 0x006EB8A0 */
extern s32 fxWallModel;            /* 0x006F2220 archive entry 9 */

/* One wall shard (FxWallShards.shards, 20 of them). */
typedef struct FxWallShard {
    /* 0x00 */ GphModel *model;    /* BtlModelCreate */
    /* 0x04 */ f32 vel[3];
    /* 0x10 */ s8 age;
    /* 0x11 */ s8 life;
    /* 0x12 */ u8 pad12[2];
} FxWallShard; /* size 0x14 */

/* Wall shard burst (FxWallShardsStart / Update / Register), 16 x 0x10. */
typedef struct FxWallShards {
    /* 0x0 */ s8 kind;             /* 1 hit, 2 wall broken (screen flash) */
    /* 0x1 */ s8 state;            /* 1 create, 2 flying, 3 free */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ s32 pool;            /* MemAlloc128; void * in 6A2120, 6A2178 */
    /* 0x8 */ s32 stamp;
    /* 0xC */ FxWallShard *shards; /* MemAlloc16C, 20 x 0x14 */
} FxWallShards; /* size 0x10 */

extern FxWallShards fxWallShards[16]; /* 0x006EBA20 */
extern s32 fxWallShardModel;       /* 0x006F2224 archive entry 10 */

/* Regen sparkle (FxRegenUpdate / Draw), 31 per burst. */
typedef struct FxRegenPart {
    /* 0x00 */ u16 angle;          /* s16 in 6A27F0 (only written there) */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 pos[3];
    /* 0x14 */ f32 vel[3];
} FxRegenPart; /* size 0x20 */

typedef struct FxRegen {
    /* 0x0 */ FxRegenPart *parts;  /* MemAlloc16C(0x3E0); NULL when idle */
    /* 0x4 */ s8 delay;            /* frames to the next burst */
    /* 0x5 */ s8 timer;            /* burst frames left (30); u8 in 6A2A48 */
    /* 0x6 */ u8 pad6[2];
} FxRegen; /* size 0x8 */

extern FxRegen fxRegen[16];        /* 0x006EBB20 */

/* Anti-magic ring particle (FxAntiMagicUpdate), 30 per unit. */
typedef struct FxAntiMagicPart {
    /* 0x0 */ s8 life;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u16 angle;           /* read unsigned (FxAntiMagicDraw) */
    /* 0x4 */ f32 scale;
} FxAntiMagicPart; /* size 0x8 */

typedef struct FxAntiMagic {
    /* 0x0 */ FxAntiMagicPart *parts;
    /* 0x4 */ s8 next;             /* next part to spawn */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 angle;           /* ring rotation */
} FxAntiMagic; /* size 0x8 */

extern FxAntiMagic fxAntiMagic[16]; /* 0x006EBBA0 */

/* Status-0x80 ring particle (FxStunUpdate), 16 per unit. */
typedef struct FxStunPart {
    /* 0x0 */ s8 life;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u16 angle;           /* read unsigned (FxStunDraw) */
    /* 0x4 */ f32 scale;
    /* 0x8 */ GphAnimState anim;   /* ptclAnimSetTex38 */
} FxStunPart; /* size 0x10 */

typedef struct FxStun {
    /* 0x0 */ FxStunPart *parts;
    /* 0x4 */ s8 next;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 angle;
} FxStun; /* size 0x8 */

extern FxStun fxStun[16];          /* 0x006EBC20 */

/* Bind particle (FxBindUpdate / Draw), 32 per unit. */
typedef struct FxBindPart {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 vel[3];
    /* 0x18 */ f32 sx;
    /* 0x1C */ f32 sy;
    /* 0x20 */ GphAnimState anim;  /* ptclAnimSetTex38 */
    /* 0x28 */ s8 life;
    /* 0x29 */ u8 flip;            /* SprState.flags bits */
    /* 0x2A */ u16 angle;          /* s16 in 6A3A18 */
} FxBindPart; /* size 0x2C */

typedef struct FxBind {
    /* 0x0 */ FxBindPart *parts;
    /* 0x4 */ s8 next;
    /* 0x5 */ u8 pad5[3];
} FxBind; /* size 0x8 */

extern FxBind fxBind[16];          /* 0x006EBCA0 */

/* Defeat burst particle (FxDefeatBurstStart / Update / Draw), 15 per unit. */
typedef struct FxDefeatPart {
    /* 0x00 */ s8 age;
    /* 0x01 */ s8 life;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 sx;
    /* 0x08 */ f32 sy;
    /* 0x0C */ f32 pos[3];
    /* 0x18 */ f32 vel[3];         /* added to pos each frame (FxDefeatBurstUpdate) */
} FxDefeatPart; /* size 0x24 */

typedef struct FxDefeat {
    /* 0x0 */ FxDefeatPart *parts; /* BtlAlloc32(0x21C) */
    /* 0x4 */ s8 unk4;
    /* 0x5 */ s8 unk5;
    /* 0x6 */ u8 pad6[2];
} FxDefeat; /* size 0x8 */

extern FxDefeat fxDefeat[16];      /* 0x006EBD20 */

/* Combo link ring (FxComboLinkSet / Update), 90 per link. */
typedef struct FxComboLinkPart {
    /* 0x0 */ s8 life;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 member;          /* party member it runs towards */
    /* 0x4 */ f32 radius;
} FxComboLinkPart; /* size 0x8 */

typedef struct FxComboLink {
    /* 0x00 */ FxComboLinkPart *parts; /* s32 in 6A04B8, 6A0870, 6A08B0 (fxComboLink) */
    /* 0x04 */ BtlUnit *actor;
    /* 0x08 */ s8 next;
    /* 0x09 */ u8 members;         /* party partner mask */
    /* 0x0A */ s8 ending;
    /* 0x0B */ u8 padB;
    /* 0x0C */ f32 radius;         /* grows by 15 a frame */
    /* 0x10 */ f32 maxDist;
    /* 0x14 */ f32 dist[4];        /* to each partner */
    /* 0x24 */ u8 pad24[0xC];
} FxComboLink; /* size 0x30 */

extern FxComboLink fxComboLink;    /* 0x006EBDA0 */

/* Weapon aura emitter (FxWeaponAura*), 16 x 0x640. */
typedef struct FxWeaponAura {
    /* 0x000 */ s8 kind;           /* burst type 1-16, 0 off */
    /* 0x001 */ s8 stop;
    /* 0x002 */ s8 dirMode;        /* FxWeaponAuraSetDir */
    /* 0x003 */ s8 pause;          /* 1 at start, -1 paused */
    /* 0x004 */ u8 timer;          /* s8 in 6A55B0 (only cleared there) */
    /* 0x005 */ u8 interval;
    /* 0x006 */ u8 unk6;
    /* 0x007 */ u8 pad7;
    /* 0x008 */ s16 unk8;
    /* 0x00A */ s16 unkA;
    /* 0x00C */ s16 unkC;
    /* 0x00E */ u8 padE[2];
    /* 0x010 */ f32 unk10;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 seg0[3];       /* weapon segment ends (FxWeaponAuraSetSegment), model scaled */
    /* 0x024 */ f32 seg1[3];
    /* 0x030 */ f32 dir[3];
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ PtclBtlSpr sprites[0x18];
} FxWeaponAura; /* size 0x640 */

extern FxWeaponAura fxWeaponAura[16]; /* 0x006EBDD0 */

/* ------------------------------------------------------------------ */
/* IP gauge and party status panels (Ip*, 0x006A6BC8-0x006AA078)       */
/* ------------------------------------------------------------------ */

/* Per-unit icon on the IP dial (IpIconUpdate / Bounce / SetPos). */
typedef struct IpIcon {
    /* 0x00 */ f32 pos;            /* 0-100 along the dial */
    /* 0x04 */ f32 bounceVel;      /* s32 in 6A7CF8 (not read there) */
    /* 0x08 */ f32 bounce;         /* radial offset */
    /* 0x0C */ s32 flash;
    /* 0x10 */ f32 spread;         /* combo spread radius */
    /* 0x14 */ s32 spreadAngle;
} IpIcon; /* size 0x18 */

/* Combo participant shown on the gauge. */
typedef struct IpComboPreview {
    /* 0x0 */ BtlUnit *unit;        /* NULL = end of the list */
    /* 0x4 */ f32 ipPos;
    /* 0x8 */ s32 timer;
} IpComboPreview; /* size 0xC */

/* IP gauge state (0x006F2238) with the per-unit icons and the combo preview
 * (6A6DD0, 6A6E00 and IpGaugeDraw reach the icons through ipGauge). */
typedef struct IpGauge {
    /* 0x000 */ f32 highlight;     /* actor highlight, fades from 1 */
    /* 0x004 */ s32 actor;         /* acting unit, -1 none */
    /* 0x008 */ s32 targetMask;    /* units flashed as targets */
    /* 0x00C */ s32 areaMask;      /* units in the target's area (| 0x10000) */
    /* 0x010 */ s32 shown;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ f32 slide;         /* 224 hidden, 0 shown */
    /* 0x01C */ s8 comboMark;      /* 1 on, 2 fading */
    /* 0x01D */ u8 pad1D[3];
    /* 0x020 */ IpIcon icons[16];
    /* 0x1A0 */ IpComboPreview combo[4];
} IpGauge; /* size 0x1D0 (ipPanels follows) */

/* The symbol files also name ipGaugeActor (0x006F223C) and ipGaugeShown
 * (0x006F2248): their users match through ipGauge's fields. ipIcons and
 * ipComboPreview label ipGauge.icons and ipGauge.combo; 6A6F10 and 6A78E8
 * only match through these names (gcse hash order). */
extern IpGauge ipGauge;            /* 0x006F2238 */
extern IpIcon ipIcons[16];         /* 0x006F2258 = ipGauge.icons */
extern IpComboPreview ipComboPreview[4]; /* 0x006F23D8 = ipGauge.combo */

typedef struct IpPoint {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
} IpPoint; /* size 0x8 (copied as a struct by IpPanelSetMode) */

/* Party status panel (IpPanelSetMode / Update / Draw), 4 x 0xE8. */
typedef struct IpPanel {
    /* 0x00 */ s32 mode;           /* 0/1 hidden, 0x10 slide in, 0x11 centre, 0x12 slide out */
    /* 0x04 */ s32 unk4;           /* IpPanelSetMode arg 2 */
    /* 0x08 */ s32 timer;
    /* 0x0C */ IpPoint pos;
    /* 0x14 */ IpPoint from;       /* pos when the mode changed */
    /* 0x1C */ IpPoint shake;      /* offset while shaking */
    /* 0x24 */ s8 sp;              /* shown SP bar (rolls toward the unit's SP) */
    /* 0x25 */ s8 spMax;           /* min(maxSp, 100); u8 in 6A7610 */
    /* 0x26 */ u8 spGlow;          /* bar colour, pulses at full SP */
    /* 0x27 */ s8 flash;           /* shake frames (IpPanelFlash 0x20); u8 in 6A6D90 */
    /* 0x28 */ FxStatusIcon icons[12];
} IpPanel; /* size 0xE8 */

extern IpPanel ipPanels[4];        /* 0x006F2408 */

/* IP gauge debug values (DbgBtlInfoPage); named in config/symbol_addrs.txt. */
extern s16 ipDialX;                /* 0x006DF268 "IP X" */
extern s16 ipDialY;                /* 0x006DF26A "IP Y" */
extern f32 ipDialScaleX;           /* 0x006DF26C "SX" */
extern f32 ipDialScaleY;           /* 0x006DF270 "SY" */
extern s32 ipDialAng;              /* 0x006DF274 "ANG" */
extern f32 ipTransRate;            /* 0x006F27A8 "RATE" */
extern f32 ipTransDelta;           /* 0x006F27AC "D" */
extern s32 ipTransDelay;           /* 0x006F27B0 "CNG" */
extern f32 ipTransNextDelta;       /* 0x006F27B4 second "D" */

#endif /* BATTLE_N30_H */
