/* Port-only C for data the decompilation still has in assembly (asm/main/data/main/0A8D68.rodata.s). */
#include "common.h"

const char _sceKernelHelperErrStr[0x28] = "## internel error in libkernl.a!\n";

const char _sceTtyPacketSizeErrStr[0x28] = "TTY: packet size larger than expect\n";

const char _sceTtyRecvErrStr[0x18] = "TTY: receive error";

const char _sceTtySendErrFmt[0x18] = "TTY: send err %d\n";

const char _sceTtyWlenErrFmt[0x18] = "TTY: err ti->wlen=%08x\n";
