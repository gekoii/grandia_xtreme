#ifndef MAIN_N08_H
#define MAIN_N08_H

#include "common.h"
#include "gph.h"   /* GphScreenFx */
#include "sdk/libdma.h"
#include "mc.h"    /* McPortState */

/*
 * Main executable 0x00453E78-0x0045D310: the screen-effect controller
 * (GphScreenFx*), the memory card driver over libmc (Mc*, request queue and
 * per-port state machine), the DMA channel wrappers over libdma (Gs*Dma*,
 * GsMfifo*) and the GS packet builders. Layouts from
 * docs/notes/names_observations.md (n08, n12) and the code.
 */

/* ------------------------------------------------------- screen effects */

/* GphScreenFx and the GPH_SFX_* flags are in gph.h. */

void GphScreenFxSet(GphScreenFx *fx, u8 type, s32 on, u32 frames, f32 val);
void GphScreenFxUpdate(GphScreenFx *fx);
void GphScreenFxDraw(GphScreenFx *fx, s32 captureAddr);

/* ------------------------------------------------------------ memory card */

/* McPortState, MC_STATE_*, mcPortState and mcCurPort are in mc.h. */

/* McReq.cmd: McExecNext's libmc call */
#define MC_CMD_GETINFO     1
#define MC_CMD_OPEN        2
#define MC_CMD_CLOSE       3
#define MC_CMD_SEEK        4
#define MC_CMD_READ        5
#define MC_CMD_WRITE       6
#define MC_CMD_FLUSH       10
#define MC_CMD_MKDIR       11
#define MC_CMD_CHDIR       12
#define MC_CMD_GETDIR      13
#define MC_CMD_SETFILEINFO 14
#define MC_CMD_DELETE      15
#define MC_CMD_FORMAT      16
#define MC_CMD_UNFORMAT    17
#define MC_CMD_GETENTSPACE 18
#define MC_CMD_RENAME      19
#define MC_CMD_THREADPRIO  20

/* A queued libmc request (McReq* wrappers 455150-4555C0, McExecNext). */
typedef struct McReq {
    /* 0x00 */ s8 id;              /* index in mcReqPool */
    /* 0x01 */ s8 status;          /* 0 queued, 1 running, 2 done, -1 cancelled */
    /* 0x02 */ s8 port;
    /* 0x03 */ s8 slot;
    /* 0x04 */ s16 cmd;            /* MC_CMD_* */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 fd;             /* sceMcOpen result */
    /* 0x0C */ s32 arg[4];         /* the libmc call's arguments after port/slot */
    /* 0x1C */ s32 result;         /* sceMcSync result */
} McReq; /* size 0x20 */

typedef struct McReqSlot {
    /* 0x00 */ s32 used;
    /* 0x04 */ McReq req;
} McReqSlot; /* size 0x24 */

/* Request pool (mcReqPool; McReset, McReqAlloc/Free/Lookup). McReqIsDone
 * 4548F8 and McReqGetStatus 454960 keep a local view: they index 0x24-byte
 * records from the pool's start (used at +4, status at +9), and with
 * &slots[i] gcc adds the +4 to the base instead of folding it. */
typedef struct McReqPool {
    /* 0x000 */ s32 count;         /* 16 */
    /* 0x004 */ McReqSlot slots[16];
} McReqPool; /* size 0x244 */

/* Pending requests in order (mcReqQueue; McQueuePush/Pop). */
typedef struct McReqQueue {
    /* 0x00 */ s32 count;          /* 32 */
    /* 0x04 */ McReq *slots[32];   /* slots[0] is mcReqQueueHead */
} McReqQueue; /* size 0x84 */

extern McReqPool mcReqPool;             /* 0x004F5FE8 */
extern McReqQueue mcReqQueue;           /* 0x004F64D0 */
extern McReq *mcReqQueueHead;           /* 0x004F64D4 = mcReqQueue.slots[0] */
extern s32 mcPollInterval;              /* 0x004A94B4 */
extern s32 mcBusy;                      /* 0x0049B240 */
extern s32 mcSyncPort;                  /* 0x0049B244 */
extern s32 mcPollFrames;                /* 0x0049B248 */
extern s32 mcBusyFrames;                /* 0x0049B24C */
extern s32 mcScanBusyFrames;            /* 0x0049B250 */
extern McReq *mcCurReq;                 /* 0x004DB8B4 */
extern s32 mcSyncResult;                /* 0x004DB8B8 */

McReq *McReqAlloc(void);
McReq *McReqLookup(s32 id);
McReq *McReqGet(s32 id);
s32 McPortPoll(McPortState *ps);

/* ------------------------------------------------------------------ libdma */

/* The libdma types (tD_CHCR, sceDmaChan, sceDmaEnv) are in sdk/libdma.h.
 * sceDmaGetChan returns sceDmaChan * (GsDmaInit 455D68 only matches with
 * that prototype). The wait loops
 * test CHCR.STR through a volatile u32 cast: the bitfield gives srl/andi. */

extern sceDmaChan *gsDmaChanVif0;       /* 0x004A94F0, sceDmaGetChan(0) */
extern sceDmaChan *gsDmaChanVif1;       /* 0x004A94E8, sceDmaGetChan(1) */
extern sceDmaChan *gsDmaChanGif;        /* 0x004A94EC, sceDmaGetChan(2) */
extern sceDmaChan *gsDmaChanFromSpr;    /* 0x004A94F4, sceDmaGetChan(8) */
extern sceDmaChan *gsDmaChanToSpr;      /* 0x004A94F8, sceDmaGetChan(9) */
extern sceDmaEnv gsDmaEnv;              /* 0x00577740, the MFIFO ring */

/* ---------------------------------------------------------- GS packets */

/*
 * Head of the packets the Gs*PacketSet builders fill and GsMfifoSend sends:
 * a DMA cnt tag, an id in its upper half, VIF NOPs and DIRECT, then an A+D
 * GIF tag. The register/value pairs follow.
 */
typedef struct GsPacketHead {
    /* 0x00 */ u64 dmaTag;         /* cnt, qword count */
    /* 0x08 */ u64 id;             /* 0xEEEE0000 + packet kind */
    /* 0x10 */ u32 vif[4];         /* 0, 0, 0, DIRECT */
    /* 0x20 */ u64 gifTag[2];      /* NLOOP | EOP, A+D */
} GsPacketHead; /* size 0x30 */

#endif
