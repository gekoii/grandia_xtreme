#ifndef FIELD_EVENT_H
#define FIELD_EVENT_H

#include "common.h"

#include "vu.h"
#include "field/occ.h"   /* OccEntry, OccsEntry, OccTable, OccsTable */

struct GphModel;

/*
 * FIELD scenario scripts, messages, event areas and OCC/OCCS triggers.
 *
 * EvtScnrWorkLayout (0x006600C0) carves one work buffer into the scenario
 * globals: evtScnrWork = work, evtScnrCur = +0x474, evtMsgCtl = +0x488,
 * evtMsgWins = +0x5C4, evtScnrRegSlots = +0x5FC, evtScnrGameRefs = +0x67C,
 * evtScnrSndStatus = +0x694, evtMsgCursor = +0x770, evtScnrBlock790 = +0x790,
 * evtScnrInput = +0x79E, evtPictureReqs = +0x7AE, evtScnrBlock7D8 = +0x7D8,
 * evtTenantPointReqs = +0x808, evtScnrCmdQueue = +0xA08,
 * evtScnrSysVarIds = +0x2308, evtScnrWorkEnd = +0x231C.
 */

/* ---------------------------------------------------------------- scenario */

/* EvtScnrWork.flags */
#define EVT_SCNR_TIMERS_FAST 0x1   /* timers x1.2 */
#define EVT_SCNR_DEBUG       0x8

typedef struct EvtScnrWork {
    /* 0x000 */ u8 *pc;            /* s32 in 6 views */
    /* 0x004 */ u16 *index;        /* {id, offset} script index */
    /* 0x008 */ u8 *body;          /* script body */
    /* 0x00C */ u8 *vars;          /* variable/flag area (save +0x1E38) */
    /* 0x010 */ s32 sysVars;       /* = work + 0x1308 */
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ u8 *scripts[256];  /* from the map header +0x7C (0x00660720) */
    /* 0x418 */ u16 dbgCmd;        /* EvtScnrDebugSetCmd */
    /* 0x41A */ u8 pad41A[2];
    /* 0x41C */ u32 dbgArgs[6];
    /* 0x434 */ u8 pad434[0x10];
    /* 0x444 */ u16 flags;         /* EVT_SCNR_* */
    /* 0x446 */ u8 unk446;
    /* 0x447 */ u8 unk447;         /* s8 in 0x00661FA8 */
    /* 0x448 */ u8 winKind;
    /* 0x449 */ u8 unk449;
    /* 0x44A */ u8 unk44A;
    /* 0x44B */ u8 pad44B;
    /* 0x44C */ s16 textColor;     /* u16 in 0x00662A40 */
    /* 0x44E */ u8 pad44E[2];
    /* 0x450 */ f32 waitTimer;
    /* 0x454 */ u8 streamWait;     /* 1 waits for the stream evtScnrSndStatus[streamIndex] */
    /* 0x455 */ u8 pad455;
    /* 0x456 */ u16 streamIndex;
    /* 0x458 */ u8 pad458[2];
    /* 0x45A */ u16 idBase;        /* added to OCC ids and map-jump numbers (high byte from the script) */
    /* 0x45C */ s8 occWakeKind;    /* EvtScnrOccWake* (0x00662830); -1 none */
    /* 0x45D */ u8 pad45D;
    /* 0x45E */ s16 occWakeId;
    /* 0x460 */ s8 occWaitKind;    /* EvtScnrOccWait* */
    /* 0x461 */ u8 pad461;
    /* 0x462 */ s16 occWaitId;
    /* 0x464 */ s8 occWaitState;
    /* 0x465 */ u8 pad465[3];
    /* 0x468 */ u8 lastWinCode;    /* 0xFF none */
    /* 0x469 */ u8 pad469[3];
    /* 0x46C */ s32 unk46C;        /* queued evtTenantPointReqs (EvtScnrExec, 0x005FC978) */
    /* 0x470 */ s32 cmdCount;      /* queued commands (evtScnrCmdQueue) */
} EvtScnrWork; /* size 0x474 */

/* flags with the halfword after it, as one word: EvtScnrMain and 0x00661140
 * test flags & 0xC0 with an lw. */
#define EVT_SCNR_FLAGS_WORD(w) (*(u32 *)&(w)->flags)

typedef struct EvtScnrCur {
    /* 0x00 */ u16 id;             /* 0xFFFF none; 0xE000-0xEFFF notices */
    /* 0x02 */ u16 flags;          /* s16 in 0x006602A0 */
    /* 0x04 */ u8 winKind;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u16 unk6;           /* s16 in 0x006602A0 */
    /* 0x08 */ u8 condResult;      /* s8 in 0x00662BB8 */
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 textSpeed;
    /* 0x0B */ u8 padB;
    /* 0x0C */ s16 noticeTimer;    /* notice auto-close after 0x78 frames; u16 in 0x00663F68 */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u8 *pc;             /* s32 in 0x006602A0/0x00662900 */
} EvtScnrCur; /* size 0x14 */

/* ---------------------------------------------------------------- messages */

typedef struct EvtMsgCtl {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 state;          /* 0xFF closed */
    /* 0x002 */ u16 flags;
    /* 0x004 */ u16 flags2;
    /* 0x006 */ u16 unk6;
    /* 0x008 */ s16 cursorX;
    /* 0x00A */ s16 cursorY;
    /* 0x00C */ u16 unkC;
    /* 0x00E */ s16 unkE;          /* u16 in 3 views */
    /* 0x010 */ s16 unk10;
    /* 0x012 */ s16 unk12;
    /* 0x014 */ s16 unk14;
    /* 0x016 */ s16 unk16;
    /* 0x018 */ s16 choice;
    /* 0x01A */ s16 choiceCount;
    /* 0x01C */ f32 wait;          /* s32/void * where only zeroed */
    /* 0x020 */ s16 unk20;
    /* 0x022 */ s16 unk22;
    /* 0x024 */ u8 unk24;
    /* 0x025 */ u8 pad25[3];
    /* 0x028 */ u8 *textPc;        /* s32 in its 3 views; must share one type with +0x2C..+0x34 (0x006602F0) */
    /* 0x02C */ u8 *ret[2];        /* return stack */
    /* 0x034 */ u8 *unk34;
    /* 0x038 */ u8 name[0x100];    /* glyph buffer */
    /* 0x138 */ u8 pad138[4];
} EvtMsgCtl; /* size 0x13C */

typedef struct EvtMsgWin {
    /* 0x00 */ u8 layout;          /* 0xFF none */
    /* 0x01 */ u8 flags;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 x0;             /* read as u16 in 0x005C8A68 */
    /* 0x08 */ s32 y0;
    /* 0x0C */ s32 x1;
    /* 0x10 */ s32 y1;
    /* 0x14 */ s32 width;
    /* 0x18 */ s32 lines;
} EvtMsgWin; /* size 0x1C */

/* Page cursor request of the scenario (evtMsgCursor = scenario work +0x770;
 * EvtMsgPromptSet 0x006626F8 sets it, TxtMsgCursorUpdate 0x005D1258 shows it). */
typedef struct EvtMsgCursor {
    /* 0x0 */ u8 on;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
} EvtMsgCursor; /* size 0x8 */

/* ---------------------------------------------------------------- event areas */

/* EvtArea.shape */
#define EVT_AREA_BOX      0
#define EVT_AREA_CAPSULE  1
#define EVT_AREA_CYLINDER 2
#define EVT_AREA_SPHERE   3

/* EvtArea.flags */
#define EVT_AREA_ENABLED 0x1
#define EVT_AREA_SCALED  0x8

typedef struct EvtArea {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 id;
    /* 0x04 */ s8 state;
    /* 0x05 */ s8 target;          /* what it tests against */
    /* 0x06 */ s8 shape;           /* EVT_AREA_* */
    /* 0x07 */ u8 flags;
    /* 0x08 */ s8 mode;
    /* 0x09 */ u8 chk;
    /* 0x0A */ s8 unkA;
    /* 0x0B */ s8 result;          /* hit result; u8 in 0x005B2418 */
    /* 0x0C */ struct GphModel *link; /* object the area moves with */
    /* 0x10 */ u16 start;
    /* 0x12 */ s16 count;
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ FVECTOR pos;
    /* 0x30 */ FVECTOR move;       /* per-frame move delta */
    /* 0x40 */ FVECTOR ofs;
    /* 0x50 */ f32 scale[3];
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ s32 rot[3];
    /* 0x6C */ f32 radius;         /* bounding sphere */
    /* 0x70 */ u8 pad70[0x10];
    /* 0x80 */ FVECTOR mtx[4];     /* world matrix (EvtAreaCalcMatrix) */
    /* 0xC0 */ union {
        f32 box[12];
        struct {
            FVECTOR a;
            FVECTOR b;
        } capsule;
        struct {
            FVECTOR ofs;
            f32 radius;
            f32 height;
        } cylinder;
    } shapeData;
    /* 0xF0 */ u8 padF0[0x10];
} EvtArea; /* size 0x100 */

typedef struct EvtAreaTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ EvtArea *areas;
} EvtAreaTable;

/* ---------------------------------------------------------------- smaller work blocks */

/* Stream status records (evtScnrSndStatus = work +0x694): [0] the BGM, [1] the
 * second stream, filled by FldSndUpdate 0x00617D78; script op / message code
 * 29 waits on the play time. EvtSndSlotsResetAll (0x00660400) resets 11
 * records, EvtSndSlotsReset (0x00660438) 9. */
typedef struct EvtScnrSndStatus {
    /* 0x00 */ s32 id;             /* -1 none */
    /* 0x04 */ s32 unk4;           /* -1 none */
    /* 0x08 */ s32 state;          /* 1 playing, 2 done */
    /* 0x0C */ s32 time;           /* play time (x1.2 with EVT_SCNR_TIMERS_FAST) */
    /* 0x10 */ s32 unk10;
} EvtScnrSndStatus; /* size 0x14 */

/* evtScnrBlock790 = work +0x790 (EvtScnrBlock790Reset 0x006605F0). */
typedef struct EvtScnrBlock790 {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ u8 unk6[4];
    /* 0xA */ u8 unkA[4];
} EvtScnrBlock790; /* size 0xE */

/* Tenant point request (evtTenantPointReqs = work +0x808, 64 entries):
 * EvtScnrExec queues them (EvtScnrWork.unk46C counts them),
 * EvtTenantPoseRequestFlush 0x005FC978 moves the tenants. */
typedef struct EvtTenantPointReq {
    /* 0x0 */ u16 tenant;
    /* 0x4 */ s32 point;
} EvtTenantPointReq; /* size 0x8 */

/* ---------------------------------------------------------------- globals */

extern EvtScnrWork *evtScnrWork;
extern EvtScnrCur *evtScnrCur;
extern EvtMsgCtl *evtMsgCtl;
extern EvtMsgWin *evtMsgWins;  /* 2 slots */
extern EvtMsgCursor *evtMsgCursor;
extern EvtScnrSndStatus *evtScnrSndStatus;
extern EvtScnrBlock790 *evtScnrBlock790;
extern s32 *evtScnrBlock7D8;               /* 12 words (EvtScnrBlock7D8Reset 0x006606E0) */
extern EvtTenantPointReq *evtTenantPointReqs;
extern EvtAreaTable *evtAreaTable;
extern OccTable *occTable;
extern OccsTable *occsTable;

#endif /* FIELD_EVENT_H */
