#ifndef MC_H
#define MC_H

/*
 * Memory card: the save-file table (McInit 0x0042C578, McSetSaveData
 * 0x0042C628), the per-port card state (McGetPortState / McGetCurPort) and
 * the save slot table (McGetSlotTable 0x0042CAF0) filled by McScanSlots
 * 0x0042CDB8 and read by FIELD's save/load flows (field/n26.h). The libmc
 * request queue is in main/n08.h, the debug page and directory listing in
 * main/n03.h.
 */

#include "common.h"
#include "save.h"

/* ---------------------------------------------------------- save-file table */

/* Save-file table (0x0049A848, 3 x 0x28): "icon.sys", "icon0.ico" and the
 * save file. McInit 42C578 fills entry 1, McSetSaveData 42C628 entry 2. */
typedef struct McFileEntry {
    /* 0x00 */ char name[0x20];
    /* 0x20 */ s32 size;
    /* 0x24 */ void *data;
} McFileEntry; /* size 0x28 */

extern McFileEntry mcIconSysEntry;      /* 0x0049A848, entry 0 "icon.sys" */
extern McFileEntry mcIconFileEntry;     /* 0x0049A870, entry 1 "icon0.ico" */
extern McFileEntry mcSaveFileEntry;     /* 0x0049A898, entry 2 (saveNN.dat) */
extern s32 mcIconSysSize;               /* 0x0049A868 = mcIconSysEntry.size */
extern s32 mcIconFileSize;              /* 0x0049A890 = mcIconFileEntry.size */
extern s32 mcSaveDataSize[];            /* 0x0049A8B8 = mcSaveFileEntry.size (0x3300); an array: 42CDB8 only matches with one */
extern SysSaveData *mcSaveDataPtr;      /* 0x0049A8BC = mcSaveFileEntry.data */

/* ---------------------------------------------------------- card state */

/*
 * Per-port card state (mcPortState[2]; McPortSync, McUpdate, McPortPoll),
 * returned by McGetPortState / McGetCurPort.
 */

/* McPortState.state */
#define MC_STATE_NONE  0    /* no card (or a GetInfo poll pending) */
#define MC_STATE_NEW   1    /* card found, GetInfo to run */
#define MC_STATE_READY 2

typedef struct McPortState {
    /* 0x00 */ s32 state;          /* MC_STATE_* */
    /* 0x04 */ s32 port;
    /* 0x08 */ s32 slot;
    /* 0x0C */ s32 formatted;      /* sceMcGetInfo format (sync result -1 -> 1, -2 -> 0); mcFormatNames index */
    /* 0x10 */ s32 type;           /* sceMcGetInfo type; mcCardTypeNames index */
    /* 0x14 */ s32 lastCmd;        /* sceMcSync command; mcCmdNames index */
    /* 0x18 */ s32 result;         /* sceMcSync result ("Cmd_St") */
    /* 0x1C */ s32 busyFrames;     /* frames the last command ran ("Time") */
    /* 0x20 */ s32 freeKB;         /* sceMcGetInfo free space ("Free") */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 syncResult;     /* McUpdate's sceMcSync return */
    /* 0x2C */ s32 pollTimer;      /* frames since the last poll (McScanPorts) */
} McPortState; /* size 0x30 */

extern McPortState mcPortState[2];      /* 0x004F6470 */
extern McPortState mcPortState1;        /* 0x004F64A0 = mcPortState[1] */
extern McPortState *mcCurPort;          /* 0x004A94B0 */

/* (typedef names here: 42C578 doesn't match with `struct McPortState *`) */
extern McPortState *mcCardInfo0;        /* 0x004A9410, port 0 (McInit) */
extern McPortState *mcCardInfo1;        /* 0x004A9414, port 1 */

void McDbgDrawCardInfo(s32 x, s32 y, McPortState *info);

/* ---------------------------------------------------------- slot table */

/* One save slot (saveNN) of the slot table: McScanSlots 42CDB8 fills
 * used/status/size, McSlotCopyHeader 42CB00 the rest from the file's
 * header. */
typedef struct McSlot {
    /* 0x00 */ s8 used;         /* a saveNN file exists; s8: the FIELD readers (main only stores) */
    /* 0x01 */ s8 status;       /* 0 valid, -1 bad size or checksum */
    /* 0x02 */ s8 unk2;         /* save header +0x20 (low byte); s8 like used */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s32 size;        /* file size */
    /* 0x08 */ s32 playTime;    /* save header +0x1C */
    /* 0x0C */ s32 partyCount;  /* 1-4: first empty party[] entry */
    /* 0x10 */ u8 party[4];     /* save header headParty */
    /* 0x14 */ u16 level[4];    /* save header headLevel */
} McSlot; /* size 0x1C */

/* A game file found in the card's directory listing (McSaveCheckGameFiles
 * 0x0065DD40, McSaveWriteIconFiles 0x0065DF18). */
typedef struct McGameFileStat {
    /* 0x0 */ s8 used;                   /* listed */
    /* 0x1 */ s8 status;                 /* 0 right size and attributes, -1 rewrite it */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ s32 size;
} McGameFileStat; /* size 0x8 */

/* Save slot table (mcSlotTable 0x004B05D8), McGetSlotTable 42CAF0.
 * McSlotTableClear 42CD38 clears the used/status bytes of the three
 * McGameFileStat records. */
typedef struct McSlotTable {
    /* 0x00 */ s8 count;                 /* 8 */
    /* 0x01 */ u8 dirExists;             /* the game directory is on the card (McSaveReadDir, McSaveWrite) */
    /* 0x02 */ u8 fileMask;              /* game files found intact: 1 icon.sys, 2 icon0.ico, 4 save directory entry */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ McGameFileStat iconSys;
    /* 0x0C */ McGameFileStat iconFile;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ McGameFileStat gameFile;  /* the "BASLUS-20417GrandiaX" entry */
    /* 0x2C */ McSlot slots[8];
} McSlotTable; /* size 0x10C */

extern McSlotTable mcSlotTable;

McSlotTable *McGetSlotTable(void);

#endif /* MC_H */
