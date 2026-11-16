#ifndef GRXSYS_H
#define GRXSYS_H

/*
 * SYS/GRXSYS.PAC: the system data pack (fonts, textures, models, animation
 * sets, message tables and the game tables). SysDataInit (0x0041D1B8)
 * decompresses it and turns the header's offsets into pointers; sysGrxsysPac
 * points to the header. Entries +0x6C..+0xA8 stay offsets: SysDataInit adds
 * the base and stores each table in its own global (ptySkillBookTypeTable,
 * ptyEggFusionTable, ...; see party.h), so code never reads them from here.
 *
 * Data entries (textures, model packs, animation sets) are s32 as in most
 * views: they are passed to functions taking s32, and 6BC620 and 656948
 * stop matching when they are pointers. +0x148 is the exception (MapMiniInit
 * needs a pointer); +0x08, +0xF4 and +0x104 are typed pointers too (their
 * readers only load them). The glyph table and the sub-tables are typed.
 */

#include "common.h"

struct GphTex;
struct GphMotion;
struct GphAnimSet;

/* +0xC4: the 19 menu/message text tables (glyph = char + 0x81F). */
#define SYS_MSG_TABLE_COUNT 19
typedef struct SysMsgTables {
    /* 0x00 */ s32 tbl[SYS_MSG_TABLE_COUNT]; /* tbl[11] (+0x2C) save/load texts; void * in 657E60 */
} SysMsgTables; /* size 0x4C */

/* +0xC8: map sound environments (FldSndApplyMapEnv, DbgBattleSetup). */
typedef struct SysSndMapEnv {
    /* 0x0 */ s32 mapNo;        /* key: map number */
    /* 0x4 */ s16 envId;        /* SysSndEnv.id to apply */
    /* 0x6 */ s16 envIdAlt;     /* used when FldSndApplyMapEnv's arg1 != 0 */
} SysSndMapEnv; /* size 0x8 */

typedef struct SysSndEnv {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 vals[4];     /* copied to fldSnd */
    /* 0x18 */ s32 id;          /* type 11 = battle (DbgBattleSetup) */
    /* 0x1C */ u16 distScale;   /* SndSetDistanceScale */
    /* 0x1E */ u8 pad1E[2];
} SysSndEnv; /* size 0x20; the whole entry goes to SndSetEffect */

typedef struct SysSndEnvTable {
    /* 0x0 */ u32 mapCount;
    /* 0x4 */ u32 envCount;
    /* 0x8 */ SysSndMapEnv *maps; /* relocated by SysDataInit */
    /* 0xC */ SysSndEnv *envs;    /* relocated by SysDataInit */
} SysSndEnvTable;

/* +0xB8: SE banks per map (FldSndMakeMapSeName). */
typedef struct SysMapSe {
    /* 0x0 */ u16 mapNo;
    /* 0x2 */ s16 banks[4];     /* -1 none */
} SysMapSe; /* size 0xA */

typedef struct SysMapSeTable {
    /* 0x0 */ u32 count;
    /* 0x4 */ SysMapSe ents[1];  /* count entries */
} SysMapSeTable;

typedef struct SysGrxsysPac {
    /* 0x000 */ s32 fontTex;        /* system font texture (TxtFontImageSetup) */
    /* 0x004 */ s32 shatterPack;    /* GphShatterLoad model pack */
    /* 0x008 */ struct GphTex *tex08; /* uploaded once into sysTexUploadPacket; MapMiniInitAuto
                                     * (5FFE80) reads its pixels as a MapMiniTex file header view */
    /* 0x00C */ s32 tex65[8];       /* textures bound to ids 0x65..0x6C (SysTexturesBind) */
    /* 0x02C */ s32 texF0;          /* texture id 0xF0 (void * in 61EE68) */
    /* 0x030 */ s32 tex30;          /* (void * in 61EE68) */
    /* 0x034 */ s32 animSet34;      /* animation set with a texture at +0xC (Res * in 6585C8) */
    /* 0x038 */ s32 texF1;          /* texture id 0xF1 (void * in 61EE68) */
    /* 0x03C */ s32 unk3C;          /* not relocated */
    /* 0x040 */ s32 texEF;          /* texture id 0xEF (void * in 61EE68) */
    /* 0x044 */ s32 tex44;
    /* 0x048 */ s32 tex48;          /* (void * in 6018F0) */
    /* 0x04C */ s32 tex4C[8];       /* +0x4C..+0x68 textures (5DE398 reads +0x50..+0x64) */
    /* 0x06C */ s32 ofsSkillBookTypes; /* table offsets, see above */
    /* 0x070 */ s32 ofsEggFusion;
    /* 0x074 */ s32 ofsCombo;
    /* 0x078 */ s32 ofsCombine;
    /* 0x07C */ s32 ofsTable7C;
    /* 0x080 */ s32 ofsManaEggs;
    /* 0x084 */ s32 ofsItems;
    /* 0x088 */ s32 ofsItemSortKeys;
    /* 0x08C */ s32 ofsMoves;
    /* 0x090 */ s32 ofsCharDefs;
    /* 0x094 */ s32 ofsGrowth;
    /* 0x098 */ s32 ofsDropItems;
    /* 0x09C */ s32 ofsDropRanks;
    /* 0x0A0 */ s32 ofsTableA0;
    /* 0x0A4 */ s32 ofsSkills;
    /* 0x0A8 */ s32 ofsMoveLevelThresholds;
    /* 0x0AC */ u8 *glyphTable;     /* font glyph table, copied into each font image's +0x1C */
    /* 0x0B0 */ s32 modelPackB0;    /* FilePacModelBindTextures */
    /* 0x0B4 */ void *formations;   /* battle formations (Formation * in 6BB648) */
    /* 0x0B8 */ SysMapSeTable *mapSe; /* SE banks per map */
    /* 0x0BC */ s32 dbgFontTex;     /* DbgTextInit */
    /* 0x0C0 */ s32 portraitTable;  /* ChrPortraitTableInit */
    /* 0x0C4 */ SysMsgTables *msg;  /* message tables (Font * / Res * in the views) */
    /* 0x0C8 */ SysSndEnvTable *sndEnv; /* map sound environments */
    /* 0x0CC */ s32 texE7[7];       /* textures bound to ids 0xE7..0xED (s32 b[7] in 6BC620) */
    /* 0x0E8 */ s32 modelPackE8;    /* FilePacModelRelocate */
    /* 0x0EC */ s32 chunkEC;        /* FileChunkRelocate, then +0x10 */
    /* 0x0F0 */ s32 chunkF0;
    /* 0x0F4 */ struct GphMotion *chunkF4; /* FileChunkRelocate; fxCastAuraBurstMotion (6A0430) */
    /* 0x0F8 */ s32 texF8[3];       /* +0xF8..+0x100 textures */
    /* 0x104 */ struct GphAnimSet *animSets[6]; /* +0x104..+0x118 animation sets (FldSpriteInit, ptclAnimSetTex38..3D) */
    /* 0x11C */ s32 tex11C[5];      /* +0x11C..+0x12C textures (s32 d[6] in 6BC620 runs to +0x130) */
    /* 0x130 */ s32 texEE;          /* texture id 0xEE */
    /* 0x134 */ s32 envParamOfs;    /* battle envParamTable, not relocated */
    /* 0x138 */ s32 chunk138;
    /* 0x13C */ s32 chunk13C;
    /* 0x140 */ s32 tex140;
    /* 0x144 */ s32 unk144;         /* not relocated */
    /* 0x148 */ void *modelPack148; /* FilePacModelRelocate; a pointer, decided by MapMiniInit 600258's load order (most other entries need s32: 6BC620, 656948) */
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ s32 tex150;         /* demo prompt texture (SysDemoStart) */
    /* 0x154 */ s32 tex154;
    /* 0x158 */ s32 tex158;
    /* 0x15C */ s32 tex15C;
} SysGrxsysPac; /* size 0x160 */

extern SysGrxsysPac *sysGrxsysPac;

/* Other system tables set up from GRXSYS / GBX by SysDataInit. */
/* SYS/GBX.PAC: offset table relocated in place (battle unit data per party
 * slot and enemy). A struct pointer, not u32 *: FldBattleStart only matches
 * with the struct access (s32 * in SysDataInit). */
typedef struct SysGbxPac {
    /* 0x0 */ u32 ofs[1];       /* variable length */
} SysGbxPac;

extern SysGbxPac *sysGbxPac;
extern s32 sysGrxTable7C;       /* GRXSYS +0x7C table (no other users) */
extern s32 sysGrxTableA0;       /* GRXSYS +0xA0 table (no other users) */

#endif
