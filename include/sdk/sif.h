/*
 * Sony libkernel SIF: command and RPC layer, plus the loadfile module client.
 *
 * Layouts follow the SCE sifcmd.h / sifrpc.h types where their names are
 * known (sceSifClientData, sceSifServeData, sceSifQueueData,
 * sceSifReceiveData, sceSifCmdHdr), and ps2sdk's sifcmd.c / sifrpc.c for the
 * library's internal control blocks, whose SCE names are not known.
 */
#ifndef SDK_SIF_H
#define SDK_SIF_H

#include "common.h"

/* ---- SIF commands ---- */

/* Command packet header (0x10). SCE calls the command id fcode, ps2sdk cid. */
typedef struct sceSifCmdHdr {
    /* 0x00 */ u32 psize : 8;
    /* 0x00 */ u32 dsize : 24;
    /* 0x04 */ u32 dest;
    /* 0x08 */ s32 fcode;
    /* 0x0C */ u32 opt;
} sceSifCmdHdr;

/* sceSifCmdHdr plus one word (0x14): the packet sceSifInitCmd sends as
 * 0x80000000 (set IOP buffer) and 0x80000002 (init). */
typedef struct sceSifCmdInitPkt {
    /* 0x00 */ sceSifCmdHdr hdr;
    /* 0x10 */ void *buf;
} sceSifCmdInitPkt;

/* sceSifSetSreg command packet as the 0x80000001 handler reads it. */
typedef struct sceSifCmdSregPkt {
    /* 0x00 */ sceSifCmdHdr hdr;
    /* 0x10 */ s32 index;
    /* 0x14 */ s32 value;
} sceSifCmdSregPkt;

typedef void (*sceSifCmdHandler)(void *pkt, void *data);

/* One command handler slot (8), ps2sdk SifCmdHandlerData_t. SCE's own name for
 * this pair is sceSifCmdData, which would clash with the control block. */
typedef struct sceSifCmdHandlerData {
    /* 0x00 */ sceSifCmdHandler handler;
    /* 0x04 */ void *harg;
} sceSifCmdHandlerData;

/* The command layer's control block (0x20), ps2sdk struct sif_cmd_data.
 * Registered with sceSifSetReg(0x80000001) and passed as the handler argument
 * of the system commands. sceSifSetSysCmdBuffer (48E3C0) and
 * sceSifSetCmdBuffer (48E3A8) store their s32 arguments into the handler
 * pointers and counts. */
typedef struct sceSifCmdCtrl {
    /* 0x00 */ void *pktbuf;
    /* 0x04 */ void *cmdbuf;
    /* 0x08 */ void *iopbuf;
    /* 0x0C */ sceSifCmdHandlerData *sys_cmd_handlers;
    /* 0x10 */ u32 nr_sys_handlers;
    /* 0x14 */ sceSifCmdHandlerData *usr_cmd_handlers;
    /* 0x18 */ u32 nr_usr_handlers;
    /* 0x1C */ s32 *sregs;
} sceSifCmdCtrl;

/* 0x68-byte reset packet DMAed to the IOP by sceSifResetIop (cid 0x80000003).
 * Its header is spelled out with u64 bitfields: sceSifResetIop (493B18) only
 * matches that way (with sceSifCmdHdr's u32 bitfields the register
 * allocation changes). */
typedef struct sceSifResetPkt {
    /* 0x00 */ u64 psize : 8;
    /* 0x00 */ u64 dsize : 24;
    /* 0x04 */ void *dest;
    /* 0x08 */ s32 fcode;
    /* 0x0C */ u32 opt;
    /* 0x10 */ s32 size;
    /* 0x14 */ s32 arg;
    /* 0x18 */ char name[0x50];
} sceSifResetPkt;

/* ---- SIF DMA ---- */

/* sceSifSetDma descriptor (SCE sceSifDmaData). */
typedef struct sceSifDmaData {
    /* 0x0 */ u32 data;      /* EE source */
    /* 0x4 */ u32 addr;      /* IOP destination */
    /* 0x8 */ u32 size;
    /* 0xC */ u32 mode;
} sceSifDmaData; /* size = 0x10 */

/* Kernel calls (SCE prototypes): the returned id is 0 on failure;
 * sceSifDmaStat is negative once the transfer has finished. */
u32 sceSifSetDma(sceSifDmaData *sdd, s32 len);
u32 isceSifSetDma(sceSifDmaData *sdd, s32 len);
s32 sceSifDmaStat(u32 id);

/* ---- SIF RPC ---- */

/* sceSifRpcData (0x10): the RPC header shared by clients and receivers. */
typedef struct _sif_rpc_data {
    /* 0x00 */ void *paddr;
    /* 0x04 */ u32 pid;
    /* 0x08 */ s32 tid;
    /* 0x0C */ u32 mode;
} sceSifRpcData;

typedef void (*sceSifEndFunc)(void *);
typedef void *(*sceSifRpcFunc)(u32 fno, void *data, s32 size);

struct _sif_serve_data;

/* sceSifClientData (0x28), bound to an IOP server by sceSifBindRpc.
 * serve stays 0 until the bind completes, which is what every library
 * checks before calling. */
typedef struct _sif_client_data {
    /* 0x00 */ sceSifRpcData rpcd;
    /* 0x10 */ u32 command;
    /* 0x14 */ void *buff;
    /* 0x18 */ void *cbuff;
    /* 0x1C */ sceSifEndFunc func;
    /* 0x20 */ void *para;
    /* 0x24 */ struct _sif_serve_data *serve;
} sceSifClientData;

/* sceSifReceiveData (0x1C). */
typedef struct _sif_receive_data {
    /* 0x00 */ sceSifRpcData rpcd;
    /* 0x10 */ void *src;
    /* 0x14 */ void *dest;
    /* 0x18 */ s32 size;
} sceSifReceiveData;

struct _sif_queue_data;

/* sceSifServeData (0x44), an EE-side RPC server. */
typedef struct _sif_serve_data {
    /* 0x00 */ u32 command;
    /* 0x04 */ sceSifRpcFunc func;
    /* 0x08 */ void *buff;
    /* 0x0C */ s32 size;
    /* 0x10 */ sceSifRpcFunc cfunc;
    /* 0x14 */ void *cbuff;
    /* 0x18 */ s32 csize;
    /* 0x1C */ sceSifClientData *client;
    /* 0x20 */ void *paddr;
    /* 0x24 */ u32 fno;
    /* 0x28 */ void *receive;
    /* 0x2C */ s32 rsize;
    /* 0x30 */ s32 rmode;
    /* 0x34 */ u32 rid;
    /* 0x38 */ struct _sif_serve_data *link;
    /* 0x3C */ struct _sif_serve_data *next;
    /* 0x40 */ struct _sif_queue_data *base;
} sceSifServeData;

/* sceSifQueueData (0x18), a server queue serviced by one thread. */
typedef struct _sif_queue_data {
    /* 0x00 */ s32 key;
    /* 0x04 */ s32 active;
    /* 0x08 */ struct _sif_serve_data *link;
    /* 0x0C */ struct _sif_serve_data *start;
    /* 0x10 */ struct _sif_serve_data *end;
    /* 0x14 */ struct _sif_queue_data *next;
} sceSifQueueData;

/* RPC packet header (0x1C), ps2sdk SifRpcPktHeader_t. The packet pool entries
 * are 0x40 bytes; rec_id holds the allocation flag (0x80000000) and pkt_addr
 * points back at the entry itself. */
typedef struct sceSifRpcPktHeader {
    /* 0x00 */ sceSifCmdHdr sifcmd;
    /* 0x10 */ s32 rec_id;
    /* 0x14 */ void *pkt_addr;
    /* 0x18 */ s32 rpc_id;
} sceSifRpcPktHeader;

/* One 0x40-byte entry of the RPC packet, receive-data and client tables. */
typedef union sceSifRpcPkt {
    /* 0x00 */ sceSifRpcPktHeader hdr;
    /* 0x00 */ u8 data[0x40];
} sceSifRpcPkt;

/* The RPC layer's control block (0x2C), ps2sdk struct rpc_data. The handler
 * argument of commands 0x80000008/9/A/C. */
typedef struct sceSifRpcCtrl {
    /* 0x00 */ s32 pid;
    /* 0x04 */ sceSifRpcPkt *pkt_table;
    /* 0x08 */ s32 pkt_table_len;
    /* 0x0C */ s32 unused1;
    /* 0x10 */ s32 unused2;
    /* 0x14 */ sceSifRpcPkt *rdata_table;
    /* 0x18 */ s32 rdata_table_len;
    /* 0x1C */ sceSifRpcPkt *client_table;
    /* 0x20 */ s32 client_table_len;
    /* 0x24 */ s32 rdata_table_idx;
    /* 0x28 */ sceSifQueueData *active_queue;
} sceSifRpcCtrl;

/* ---- loadfile ---- */

/* Send/receive buffer of every loadfile RPC (0x200). Requests are
 * {result or path length, argument length, path[0xFC], arguments}; the
 * IOP value calls (sceSifIopGetVal / SetVal) use {address, type, value}. */
typedef union sceSifLoadBuf {
    struct {
        /* 0x00 */ s32 result;
        /* 0x04 */ s32 argLen;
        /* 0x08 */ char path[0xFC];
        /* 0x104 */ char args[0xFC];
    } module;
    struct {
        /* 0x00 */ s32 addr;
        /* 0x04 */ s32 type;
        /* 0x08 */ union {
            u8 b;
            u16 h;
            u32 w;
        } value;
    } val;
    /* 0x00 */ u8 raw[0x200];
} sceSifLoadBuf;

/* ---- globals ---- */

extern s32 _sceSifCmdInitDone;
extern s32 _sceSifRpcInitDone;
extern u8 _sceSifCmdPktBuf[0x80];      /* used uncached */
extern u8 _sceSifCmdRecvBuf[0x40];     /* used uncached */
extern sceSifCmdInitPkt _sceSifCmdInitPkt;
extern s32 _sceSifCmdDmacHandlerId;
extern sceSifCmdCtrl _sceSifCmdData;
/* Symbols inside _sceSifCmdData (+8, +0xC, +0x14) that some files use directly. */
extern u32 _sceSifCmdDataIopBuf;
extern sceSifCmdHandlerData *_sceSifCmdDataSysHandlers;
extern sceSifCmdHandlerData *_sceSifCmdDataUsrHandlers;
extern sceSifCmdHandlerData _sceSifSysCmdHandlers[32];
extern s32 _sceSifSregs[32];

extern sceSifRpcPkt _sceSifRpcPktTable[32];
extern sceSifRpcPkt _sceSifRpcRdataTable[32];
extern sceSifRpcPkt _sceSifRpcClientTable[32];
extern sceSifRpcCtrl _sceSifRpcData;

extern s32 _sceSifLoadBindState;
extern sceSifLoadBuf _sceSifLoadBuf;
extern char _sceSifLoadBufPath[];      /* _sceSifLoadBuf + 8 */
extern sceSifClientData _sceSifLoadClient;
extern u8 _sceSifLoadIopVersion[4];   /* "2340" */
extern sceSifResetPkt _sceSifResetCmd;

#endif
