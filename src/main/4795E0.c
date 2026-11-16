/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef unsigned int u128 __attribute__((mode(TI)));

extern volatile u128 _sceIpuInitIqMatrix[];
extern u128 _sceIpuInitVqClut[];

#define IPU_CMD (*(volatile s32 *)0x10002000)
#define IPU_CTRL (*(volatile s32 *)0x10002010)
#define IPU_IN_FIFO (*(volatile u128 *)0x10007010)

void sceIpuInit(void) {
    _sceIpuInitSetToChcr(1);
    IPU_CTRL = 0x40000000;
    while (IPU_CTRL < 0);
    IPU_CMD = 0;
    while (IPU_CTRL < 0);
    IPU_IN_FIFO = _sceIpuInitIqMatrix[0];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[1];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[2];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[3];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[4];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[4];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[4];
    IPU_IN_FIFO = _sceIpuInitIqMatrix[4];
    IPU_CMD = 0x50000000;
    while (IPU_CTRL < 0);
    IPU_CMD = 0x58000000;
    while (IPU_CTRL < 0);
    IPU_IN_FIFO = _sceIpuInitVqClut[0];
    IPU_IN_FIFO = _sceIpuInitVqClut[1];
    IPU_CMD = 0x60000000;
    while (IPU_CTRL < 0);
    IPU_CMD = 0x90000000;
    while (IPU_CTRL < 0);
    IPU_CTRL = 0x40000000;
    while (IPU_CTRL < 0);
    IPU_CMD = 0;
    while (IPU_CTRL < 0);
}
