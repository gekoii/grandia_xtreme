#ifndef FIELD_N27_H
#define FIELD_N27_H

#include "common.h"
#include "field/event.h"   /* EvtScnrWork, EvtScnrSndStatus, ... */

/*
 * FIELD scenario interpreter: script stepping, message codes and windows
 * (0x00660820-0x00664DB0), message window frame tiles (0x00664DB0-0x00665BC0).
 * EvtScnrWork, EvtScnrCur, EvtMsgCtl and EvtMsgWin are in field/event.h; this
 * header has the smaller blocks EvtScnrWorkLayout (0x006600C0) carves from
 * the same work buffer.
 */

/* ---------------------------------------------------------------- command queue */

/* Queued script command (evtScnrCmdQueue = work +0xA08, 0x40 entries, cleared
 * by EvtScnrExec every frame; EvtScnrWork.cmdCount counts them). EvtScnrExec
 * queues script ops, EvtMsgExecCode message codes 13/20/21 through
 * evtScnrMsgCmdPtr; 0x005FD000 runs them. */
typedef struct EvtScnrCmd {
    /* 0x00 */ u8 type;            /* script op / message code */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u32 args[8];
} EvtScnrCmd; /* size 0x24 */

extern EvtScnrCmd *evtScnrCmdQueue;
extern EvtScnrCmd *evtScnrMsgCmdPtr;   /* next entry for message codes (EvtMsgAnalyze) */

/* ---------------------------------------------------------------- stream status */

/* EvtScnrSndStatus / evtScnrSndStatus are in field/event.h. */

/* ---------------------------------------------------------------- game data */

/* Pointers into the game data (evtScnrGameRefs = work +0x67C). */
typedef struct EvtScnrGameRefs {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 *unk8;           /* byte read by the OCC condition 0x005B6398 */
    /* 0x0C */ u8 *flags;          /* bit array (EvtScnrExtFlagGet/Set) */
    /* 0x10 */ s32 *money;
    /* 0x14 */ u8 *itemCounts;     /* per item, capped at 99 */
} EvtScnrGameRefs; /* size 0x18 */

extern EvtScnrGameRefs *evtScnrGameRefs;

/* ---------------------------------------------------------------- system variables */

/* Variable and flag numbers the interpreter itself uses (evtScnrSysVarIds =
 * work +0x2308, set in FldInit). */
typedef struct EvtScnrSysVarIds {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 resultVar;      /* var 4: choice / test results */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u16 restartFlag;    /* flag 0xA4C: EvtScnrStart restarts a running script */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u16 autoFlag;       /* flag 0xA52: messages advance without a button */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u16 noSkipFlag;     /* flag 0xA33: the skip button is ignored */
    /* 0x12 */ u8 pad12[2];
} EvtScnrSysVarIds; /* size 0x14 */

extern EvtScnrSysVarIds *evtScnrSysVarIds;

/* ---------------------------------------------------------------- input */

/* Script buttons (work +0x79E, from 0x005FCA10). */
#define EVT_SCNR_KEY_UP      0x1
#define EVT_SCNR_KEY_DOWN    0x2
#define EVT_SCNR_KEY_CONFIRM 0x2000
#define EVT_SCNR_KEY_SKIP    0x4000

extern u16 *evtScnrInput;

/* ---------------------------------------------------------------- frame tiles */

/* 40 x 30 message window frame tiles (EvtMsgTileGridInit). */
#define EVT_MSG_TILE_COLS 40
#define EVT_MSG_TILE_ROWS 30

extern u16 *evtMsgTileGrid;

#endif
