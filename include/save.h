#ifndef SAVE_H
#define SAVE_H

/*
 * Save data: sysSaveData (0x004ED780) is the 0x3300-byte image written to
 * and read from the memory card as one block (McSetSaveData(&sysSaveData,
 * 0x3300)).
 *
 *   0x0000 header, play time, options
 *   0x002C party data       (ptyPartyData points here; PtyPartyData, party.h)
 *   0x1A18 character records (ptyCharData points here; 8 x 0x84, party.h)
 *   0x1E38 scenario work: main flag, map numbers, party ids, counters,
 *          minigame records and the scenario flag bit arrays
 *   0x2CF0 egg chart, mini-map explored grids, map checkpoint
 *
 * The scenario interpreter addresses everything from 0x1E38 on as bytes
 * from evtMainFlag (evtScnrWork +0xC). The flag arrays and a few fields have
 * their own symbols; their externs follow the struct.
 */

#include "common.h"
#include "party.h"

/* Bit arrays of the scenario flags, by condition type (EvtFlag* accessors). */
#define EVT_SYSTEM_FLAG_BYTES   0x20  /* type 1, GAME DEBUG page 4 */
#define EVT_GLOBAL_FLAG_BYTES   0x80  /* type 0, 0x400 flags */
#define EVT_SUBGAME_FLAG_BYTES  0x40  /* 0x200 flags */
#define EVT_XVS_FLAG_BYTES      0x40  /* 0x200 flags */
#define EVT_TREASURE_FLAG_BYTES 0x10  /* type 3 */
#define EVT_GAME_FLAG_BYTES     0x10  /* type 6 */
#define EVT_AREAMAP_FLAG_BYTES  0x04  /* 0x20 flags */
#define EVT_READER_FLAG_BYTES   0x02  /* 0x10 flags */
#define EVT_DUNGEON_FLAG_BYTES  0x08  /* type 5 */
#define EVT_MONSTER_FLAG_BYTES  0x20  /* type 4 */
#define EVT_LOCAL_FLAG_BYTES    0x10  /* type 2 */

#define SAVE_MINIMAP_GRIDS 48         /* 16 x 16 explored bits (0x20 bytes) per mini-map */

typedef struct SysSaveData {
    /* 0x0000 */ u32 checksum;      /* Adler-32 of +0x10.. */
    /* 0x0004 */ s32 size;          /* 0x32F0 */
    /* 0x0008 */ u8 pad8[8];
    /* 0x0010 */ u8 headParty[4];   /* card slot header: party ids (McSaveFlowInit) */
    /* 0x0014 */ u16 headLevel[4];  /* card slot header: party levels; u16 decided by McSlotCopyHeader's lhu */
    /* 0x001C */ s32 playTime;      /* sysSavePlayTime, frames (FldPlayTimeGet/Set) */
    /* 0x0020 */ s32 unk20;         /* stored as a word (634630); McSlotCopyHeader reads its low byte */
    /* 0x0024 */ u8 optVibration;   /* options kept by FldNewGameReset */
    /* 0x0025 */ u8 optCamReverse;  /* sysOptCamReverse */
    /* 0x0026 */ u8 optSoundMode;   /* SndSetOutputMode */
    /* 0x0027 */ u8 optMiniMapZoom;
    /* 0x0028 */ u8 optIpStop;      /* copied to btlHandoff.ipStopOpt */
    /* 0x0029 */ u8 optMiniMapOff;
    /* 0x002A */ u8 optMiniMap2A;
    /* 0x002B */ u8 optMiniMap2B;
    /* 0x002C */ PtyPartyData partyData;        /* ptyPartyData: gold, bag, storage, skills, eggs ... */
    /* 0x1A18 */ PtyChar chars[8];              /* ptyCharData: per-character records */
    /* 0x1E38 */ u32 mainFlag;      /* evtMainFlag "MAIN FLAG", scenario progress 0..0x2E9F; u32 decided by EvtScnrSysCmdExec's sltiu (s32 in 5C9698, 5D8DE0, 5D90B8) */
    /* 0x1E3C */ s32 unk1E3C;
    /* 0x1E40 */ s32 unk1E40;
    /* 0x1E44 */ s32 unk1E44;
    /* 0x1E48 */ s32 unk1E48;
    /* 0x1E4C */ u16 oldMapNo;      /* (s16 in 5D3778) */
    /* 0x1E4E */ u16 mapNo;         /* evtCurMapNo (s16 in 5D3778) */
    /* 0x1E50 */ u16 party[4];      /* sysSaveParty: character ids, 1-based, 0 empty (s16 in 6 views, u8 in 5EB1C8) */
    /* 0x1E58 */ u8 unk1E58;
    /* 0x1E59 */ u8 retryCount;
    /* 0x1E5A */ u8 areaMapNo;
    /* 0x1E5B */ u8 pad1E5B[2];
    /* 0x1E5D */ u8 counters[0x20]; /* scenario byte counters (cleared by scenario command) */
    /* 0x1E7D */ u8 pad1E7D[0x1F11 - 0x1E7D];
    /* 0x1F11 */ u8 xvsCharA[16];   /* XVS INFORMATION 2: two values per character */
    /* 0x1F21 */ u8 xvsCharB[16];
    /* 0x1F31 */ u8 xvsCount;
    /* 0x1F32 */ u8 pad1F32[0x1F41 - 0x1F32];
    /* 0x1F41 */ u8 unk1F41[3];
    /* 0x1F44 */ u8 unk1F44;        /* minigame A (concert) records */
    /* 0x1F45 */ u8 unk1F45;
    /* 0x1F46 */ u8 unk1F46;
    /* 0x1F47 */ u8 unk1F47;
    /* 0x1F48 */ u8 pad1F48[2];
    /* 0x1F4A */ u8 miniBallCourse; /* minigame B */
    /* 0x1F4B */ u8 miniBallRecords[4][2];
    /* 0x1F53 */ u8 pad1F53;
    /* 0x1F54 */ u8 miniBallRank;
    /* 0x1F55 */ u8 pad1F55[4];
    /* 0x1F59 */ u8 miniBallResult;
    /* 0x1F5A */ u8 pad1F5A[2];
    /* 0x1F5C */ u8 unk1F5C;        /* minigame A score */
    /* 0x1F5D */ u8 unk1F5D;
    /* 0x1F5E */ u8 pad1F5E[0x1F69 - 0x1F5E];
    /* 0x1F69 */ u8 unk1F69;        /* minigame version bytes */
    /* 0x1F6A */ u8 unk1F6A;
    /* 0x1F6B */ u8 pad1F6B[2];
    /* 0x1F6D */ u8 unk1F6D;
    /* 0x1F6E */ u8 pad1F6E;
    /* 0x1F6F */ u8 unk1F6F;        /* minigame A play count */
    /* 0x1F70 */ u8 unk1F70;
    /* 0x1F71 */ u8 systemFlags[EVT_SYSTEM_FLAG_BYTES];     /* evtSystemFlags; character joined = bit id+11 */
    /* 0x1F91 */ u8 globalFlags[EVT_GLOBAL_FLAG_BYTES];     /* evtGlobalFlags */
    /* 0x2011 */ u8 subGameFlags[EVT_SUBGAME_FLAG_BYTES];   /* evtSubGameFlags (5F0870 indexes it as globalFlags[k / 8 + 0x80]) */
    /* 0x2051 */ u8 xvsFlags[EVT_XVS_FLAG_BYTES];           /* evtXvsFlags */
    /* 0x2091 */ u8 treasureFlags[EVT_TREASURE_FLAG_BYTES]; /* evtTreasureFlags */
    /* 0x20A1 */ u8 gameFlags[EVT_GAME_FLAG_BYTES];         /* evtGameFlags */
    /* 0x20B1 */ u8 areaMapFlags[EVT_AREAMAP_FLAG_BYTES];   /* evtAreaMapFlags */
    /* 0x20B5 */ u8 readerFlags[EVT_READER_FLAG_BYTES];     /* evtReaderFlags */
    /* 0x20B7 */ u8 dungeonFlags[EVT_DUNGEON_FLAG_BYTES];   /* evtDungeonFlags */
    /* 0x20BF */ u8 monsterFlags[EVT_MONSTER_FLAG_BYTES];   /* evtMonsterFlags */
    /* 0x20DF */ u8 localFlags[EVT_LOCAL_FLAG_BYTES];       /* evtLocalFlags */
    /* 0x20EF */ u8 pad20EF[0x2CF0 - 0x20EF];
    /* 0x2CF0 */ u8 eggChart[6];    /* known egg-chart cells (ItemEggChartSetKnown) */
    /* 0x2CF6 */ u8 miniMapExplored[SAVE_MINIMAP_GRIDS][0x20]; /* MapMiniExploredGet/Set */
    /* 0x32F6 */ u16 checkpointMapNo; /* EvtSaveMapCheckpoint */
    /* 0x32F8 */ u16 unk32F8;
    /* 0x32FA */ u16 pad32FA;
    /* 0x32FC */ u16 checkpointCount;
    /* 0x32FE */ u8 pad32FE[2];
} SysSaveData; /* size 0x3300 */

extern SysSaveData sysSaveData;

/* Aliases into sysSaveData. */
extern s32 sysSavePlayTime;      /* .playTime */
extern u8 sysOptCamReverse;      /* .optCamReverse */
extern u32 evtMainFlag;          /* .mainFlag (s32 in 5ED2E0, 636D50; u8[] in 5D96F0, which takes byte offsets from it) */
extern u16 sysSaveParty[4];      /* .party */
extern u16 evtCurMapNo;          /* .mapNo (+0x1E4E) */
extern u8 evtSystemFlags[EVT_SYSTEM_FLAG_BYTES];
extern u8 evtGlobalFlags[EVT_GLOBAL_FLAG_BYTES];
extern u8 evtSubGameFlags[EVT_SUBGAME_FLAG_BYTES];
extern u8 evtXvsFlags[EVT_XVS_FLAG_BYTES];
extern u8 evtTreasureFlags[EVT_TREASURE_FLAG_BYTES];
extern u8 evtGameFlags[EVT_GAME_FLAG_BYTES];
extern u8 evtAreaMapFlags[EVT_AREAMAP_FLAG_BYTES];
extern u8 evtReaderFlags[EVT_READER_FLAG_BYTES];
extern u8 evtDungeonFlags[EVT_DUNGEON_FLAG_BYTES];
extern u8 evtMonsterFlags[EVT_MONSTER_FLAG_BYTES];
extern u8 evtLocalFlags[EVT_LOCAL_FLAG_BYTES];

#endif
