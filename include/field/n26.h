#ifndef FIELD_N26_H
#define FIELD_N26_H

/*
 * FIELD 0x0065C648-0x00660820: the memory card flows of the save/load menus
 * (boot "card with room" check McSpaceCheck*, save McSave*, load McLoad*),
 * the play-time counter and the scenario work sub-blocks that
 * EvtScnrWorkLayout (0x006600C0) carves (the rest are in field/event.h).
 * The effect presets before it (PtclPreset*, 0x006591E0-0x0065C648) use
 * PtclEmitter / PtclDustWork from ptcl.h.
 *
 * The card driver's types: the save-file table, card state and slot table
 * in mc.h, the directory listing in main/n03.h, the request records (McReq,
 * McReqGet) in main/n08.h. McGetCurPort returns the current McPortState.
 */

#include "common.h"
#include "pad.h"
#include "save.h"
#include "main/n08.h"   /* McReq; includes mc.h */
#include "main/n03.h"

/* ---------------------------------------------------------------- card check */

/* Boot check for a card with room for a save (menu screen 11, 0x0065C690).
 * mcSpaceCheckState: 0 port 0, 1 port 1, 2 card full, 3 no card, 4 done;
 * mcSpaceCheckPortState (McSpaceCheckPort): 0 card type, 0x402 read dir,
 * 0x408/0x409 free space, 0x405/0x40B no card, 0x406 full, 0x40A room. */
extern s32 mcSpaceCheckState;            /* 0x00670730 */
extern s32 mcSpaceCheckPrevState;        /* 0x00670734, 0xFFFF after reset */
extern s32 mcSpaceCheckPortPrevState;    /* 0x00670738 */
extern s32 mcSpaceCheckPortState;        /* 0x0067073C */
extern McDirList *mcSpaceCheckDirList;   /* 0x00670740 -> mcSpaceCheckDirBuf */
extern char mcSpaceCheckDirPattern[];    /* 0x00675AE8 "/BASLUS-20417GrandiaX/*" */
extern s32 mcSpaceCheckPortStateChanged; /* 0x00676334, 1 on a port state's first frame */
extern s32 mcSpaceCheckResult0;          /* 0x00679D40, port 0: -1 no card, -2 no space */
extern s32 mcSpaceCheckResult1;          /* 0x00679D44, port 1 */
extern s32 mcSpaceCheckDirReq;           /* 0x00679D48, getdir request id */
extern s32 mcSpaceCheckStateChanged;     /* 0x00679D4C, 1 on a state's first frame */
extern Pad *mcSpaceCheckPad;             /* 0x00679D50 = padPort0 */

/* ---------------------------------------------------------------- save / load */

extern u8 mcLastPort;                    /* 0x004A9434, last card port used (saved/loaded) */
extern u8 mcLastSlot;                    /* 0x004A9490, last save slot 0-7 */

/* Save flow (McSaveFlowUpdate 0x0065CF90). mcSaveState: 0 port select,
 * 0x100 card check, 0x101 read dir, 0x102 game files check, 0x104 slot scan,
 * 0x200/0x201 space checks, 0x202 wait, 2 slot select, 3 write, 4 format,
 * 0x3xx messages (0x300 no space, 0x303 write failed, 0x304 done,
 * 0x305 format?, 0x307 format failed, 0x308 overwrite?, 0x309 card out,
 * 0x30A not PS2, 0x30B quit?), 0x800 leave. */
extern s32 mcSaveSlotCursor;             /* 0x0067A1C0 */
extern s32 mcSaveSlotListMode;           /* 0x0067A1C4, 2 when a new save fits */
extern s32 mcSaveFormatReq;              /* 0x0067A1C8 */
extern s32 mcSaveFormatStep;             /* 0x0067A1CC, only cleared */
extern s32 mcSaveDirReq;                 /* 0x0067A1D0 */
extern s32 mcSaveIconReqs[3];            /* 0x0067A1D8, chdir or open / write / close ids */
extern s8 mcSaveIconStep;                /* 0x0067A1FC, McSaveWriteIconFiles step 0-8 */
extern s32 mcSaveMkdirReq;               /* 0x0067A200 */
extern s32 mcSaveWriteReqs[4];           /* 0x0067A208, chdir / open / write / close ids */
extern s8 mcSaveWriteStep;               /* 0x0067A21C, McSaveWrite step 0-5 */
extern s8 mcSaveWriteIsNew;              /* 0x0067A21D, the save file is being created */
extern s32 mcSaveState;                  /* 0x0067A220 */
extern s32 mcSavePrevState;              /* 0x0067A224 */
extern s32 mcSaveStateChanged;           /* 0x0067A228, 1 on a state's first frame */
extern s32 mcSaveChoice;                 /* 0x0067A22C, port or slot chosen */
extern s32 mcSaveMaxKB;                  /* 0x0067A230, never read */
extern s32 mcSaveInitMask;               /* 0x0067A234, 0xF, never read */
extern s8 mcSaveDone;                    /* 0x0067A238, a save succeeded */
extern s8 mcSavePort;                    /* 0x0067A239 */
extern s8 mcSaveSlot;                    /* 0x0067A23A */
extern McSlot *mcSaveSlotRec;            /* 0x0067A23C, slot table record of mcSaveSlot */
extern s32 mcSaveGameFilesKB;            /* 0x0067A240, KB used by the game's files */
extern s32 mcSaveGameFilesNeedKB;        /* 0x0067A244, icon.sys + icon0.ico + 2 */
extern s32 mcSaveFreeKB;                 /* 0x0067A248 */
extern Pad *mcSavePad;                   /* 0x0067A24C = padPort0 */
extern McSlot mcSaveSlotHeader;          /* 0x0067C5B8, this game's slot record */
extern s32 mcSavePlayTimeNow;            /* 0x0067C5C0, frames */

extern char mcSaveBadStateFmt[];         /* 0x00675B08 */
extern char mcSaveSlotNumFmt[];          /* 0x00675B28 "%02d" */
extern char mcSaveFileExt[];             /* 0x00675B30 ".dat" */
extern char mcSaveDirPattern[];          /* 0x00675B38 */
extern char mcSaveIconSysName[];         /* 0x00675B50 "icon.sys" */
extern char mcSaveIconFileName[];        /* 0x00675B60 "icon0.ico" */
extern char mcSaveGameFileName[];        /* 0x00675B70 "BASLUS-20417GrandiaX" */
extern char mcSaveFlowDirPath[];         /* 0x00675B88 "/BASLUS-20417GrandiaX" */
extern char mcSaveErr1Str[];             /* 0x00675BB8 */
extern char mcSaveBrokenNoSpaceStr[];    /* 0x00675BC0 */
extern char mcSaveCantRepairStr[];       /* 0x00675BF0 */

/* Load flow (McLoadFlowUpdate 0x0065F0B8): 0 port select, 0x100 card check,
 * 0x101 read dir, 0x104 slot scan, 2 slot select, 3 read, 0x304 done,
 * 0x308 no data, 0x309 card out, 0x30A failed, 0x800 loaded, 0x801 cancelled. */
extern s32 mcLoadPortCursor;             /* 0x0067A250 */
extern s32 mcLoadPortTimer;              /* 0x0067A254, never read */
extern s32 mcLoadSlotCursor;             /* 0x0067A258 */
extern s32 mcLoadDirReq;                 /* 0x0067A25C */
extern s32 mcLoadReadReqs[4];            /* 0x0067A260, chdir / open / read / close ids */
extern s32 mcLoadReadDelay;              /* 0x0067A270, 30 frames before the check */
extern s32 mcLoadState;                  /* 0x0067A274 */
extern s32 mcLoadPrevState;              /* 0x0067A278 */
extern s32 mcLoadStateChanged;           /* 0x0067A27C */
extern s32 mcLoadChoice;                 /* 0x0067A280 */
extern Pad *mcLoadPad;                   /* 0x0067A284 = padPort0 */
extern char mcLoadErrorMsg[];            /* 0x0067A288 */
extern s8 mcLoadPort;                    /* 0x0067A2A8 */
extern s8 mcLoadSlot;                    /* 0x0067A2A9 */

extern char mcLoadBadStateFmt[];         /* 0x00675C10 */
extern char mcLoadSlotOutStr[];          /* 0x00675C30 */
extern char mcLoadSlotNumFmt[];          /* 0x00675C48 "%02d" */
extern char mcLoadFileExt[];             /* 0x00675C50 ".dat" */
extern char mcLoadNotPs2Str[];           /* 0x00675C58 */
extern char mcLoadDirPattern[];          /* 0x00675C70 */
extern char mcLoadDirPath[];             /* 0x00675C88 */

/* ---------------------------------------------------------------- play time */

extern s32 fldPlayTimeBase;              /* 0x0067A2AC, frames at load / new game */
extern s32 fldPlayTimeStartFrame;        /* 0x0067A2B0, sysLoopFrameCount then */

/* ---------------------------------------------------------------- scenario work */

/* Registration slot (scenario work +0x5FC, 8 of them): EvtScnrRegSlotAdd
 * fills the first free one, nothing reads them. */
typedef struct EvtScnrRegSlot {
    /* 0x0 */ u8 id;                     /* 0xFF free */
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} EvtScnrRegSlot; /* size 0x10 */

/* Picture window request (scenario work +0x7AE, 3 of them; EvtScnrGetPicture
 * 0x00664AE8, drawn by 0x0061B318). */
typedef struct EvtPictureReq {
    /* 0x0 */ s16 id;                    /* -1 none; u16 in 0x0061B318 */
    /* 0x2 */ u8 state;                  /* 0xFF free, 1 shown, 2 closing */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 pos;
    /* 0x6 */ s16 unk6;                  /* 0xFF reset, counted up on a new picture */
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
    /* 0xC */ u8 padC[2];                /* no C reader or writer */
} EvtPictureReq; /* size 0xE */

extern EvtScnrRegSlot *evtScnrRegSlots;  /* 0x00676454 */
extern EvtPictureReq *evtPictureReqs;    /* 0x00676434 */

#endif /* FIELD_N26_H */
