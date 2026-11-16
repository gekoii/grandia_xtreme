/*
 * Sony libpad (the padman 0x80000100/0x80000101 version, the one ps2sdk's
 * "new" libpad follows).
 *
 * ScePadData is the IOP-written pad DMA area, ps2sdk struct pad_data with its
 * field names. The game's SDK keeps a different per-port record than
 * ps2sdk's struct pad_state (open, port, slot, padData, padBuf), so
 * ScePadPort is laid out from the code.
 */
#ifndef SDK_LIBPAD_H
#define SDK_LIBPAD_H

#include "common.h"
#include "sdk/sif.h"

/* One frame of the pad DMA area (0x80); the area holds two, and
 * _scePadGetDmaStr (479E48) returns the one with the larger frame count.
 * Its word alignment lets gcc fold `state == 6 && reqState == 2` into one
 * halfword compare; scePadGetState (479F78) matched with a byte-only local
 * view and needs nested ifs to match with this type. */
typedef struct ScePadData {
    /* 0x00 */ u8 data[32];
    /* 0x20 */ u32 actDirData[2];
    /* 0x28 */ u32 actAlignData[2];
    /* 0x30 */ u8 actData[32];       /* scePadInfoAct reads it as [8][4] */
    /* 0x50 */ u16 modeTable[4];
    /* 0x58 */ s32 frame;              /* s32: compared signed (479E48); ps2sdk has u32 */
    /* 0x5C */ u32 findPadRetries;
    /* 0x60 */ s32 length;
    /* 0x64 */ u8 modeConfig;
    /* 0x65 */ u8 modeCurId;
    /* 0x66 */ u8 model;
    /* 0x67 */ u8 buttonDataReady;
    /* 0x68 */ u8 nrOfModes;
    /* 0x69 */ u8 modeCurOffs;
    /* 0x6A */ u8 nrOfActuators;
    /* 0x6B */ u8 numActComb;
    /* 0x6C */ u8 val_c6;
    /* 0x6D */ u8 mode;
    /* 0x6E */ u8 lock;
    /* 0x6F */ u8 actDirSize;
    /* 0x70 */ u8 state;
    /* 0x71 */ u8 reqState;
    /* 0x72 */ u8 currentTask;
    /* 0x73 */ u8 runTask;
    /* 0x74 */ u8 stat70bit;
    /* 0x75 */ u8 pad75[11];
} ScePadData;

/* Actuator block scePadSetActDirect sends to the IOP by SIF DMA (0x20,
 * double-buffered on the IOP side by the low bit of count). */
typedef struct ScePadActBuf {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 data[6];
    /* 0x12 */ u8 pad12[0xE];
} ScePadActBuf;

/* Open-pad record (0x1C), _scePadPortTable[port][slot]. */
typedef struct ScePadPort {
    /* 0x00 */ ScePadData *padArea;   /* the caller's 0x100-byte, 64-aligned buffer */
    /* 0x04 */ ScePadActBuf *actBuf;
    /* 0x08 */ u8 *iopAddr;           /* IOP address of the actuator area, from the open RPC */
    /* 0x0C */ s32 dmaId;             /* last sceSifSetDma id */
    /* 0x10 */ s32 open;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} ScePadPort;

/* scePadSetVrefParam's 12 parameter bytes, copied as one block. */
typedef struct ScePadVrefParam {
    /* 0x00 */ u8 data[12];
} ScePadVrefParam;

/* Send/receive buffer of every padman call (0x80): the function number,
 * the arguments, then the result, so the result word moves with the
 * argument count. Like ps2sdk's buffer it is a union of the call layouts. */
typedef union ScePadRpcBuf {
    s32 command;
    struct { /* 0x0C scePadGetPortMax, 0x0D scePadGetSlotMax, 0x0E close, 0x0F scePadEnd, 0x10 init, 0x12 version */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 port;
        /* 0x08 */ s32 slot;
        /* 0x0C */ s32 result;
        /* 0x10 */ s32 mode;          /* 0x0E scePadPortClose: 1 */
        /* 0x14 */ s32 iopAddr;       /* scePadPortOpen result */
    } port;
    struct { /* 0x14 (_scePadRpcCmd14, uncalled) */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 arg;
        /* 0x08 */ s32 result;
    } cmd14;
    struct { /* 0x0A scePadSetButtonInfo */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 port;
        /* 0x08 */ s32 slot;
        /* 0x0C */ s32 mask;
        /* 0x10 */ s32 result;
    } buttonInfo;
    struct { /* 0x06 scePadSetMainMode */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 port;
        /* 0x08 */ s32 slot;
        /* 0x0C */ s32 offs;
        /* 0x10 */ s32 lock;
        /* 0x14 */ s32 result;
    } mainMode;
    struct { /* 0x08 scePadSetActAlign */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 port;
        /* 0x08 */ s32 slot;
        /* 0x0C */ u8 align[6];
        /* 0x12 */ u8 pad12[2];
        /* 0x14 */ s32 result;
    } actAlign;
    struct { /* 0x0B scePadSetVrefParam */
        /* 0x00 */ s32 command;
        /* 0x04 */ s32 port;
        /* 0x08 */ s32 slot;
        /* 0x0C */ ScePadVrefParam param;
        /* 0x18 */ s32 unk18;
        /* 0x1C */ s32 result;
    } vref;
    u8 raw[0x80];
} ScePadRpcBuf;

extern s32 _scePadInitDone;
extern s32 _scePadDebugPrint;
extern char *_scePadStateStrTable[8];
extern char *_scePadReqStateStrTable[3];
extern sceSifClientData _scePadClient[2];
extern ScePadPort _scePadPortTable[2][4];
extern ScePadRpcBuf _scePadRpcBuf;

#endif
