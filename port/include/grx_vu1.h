/* The port's VU1: its memories, which the VIF1 layer (port/src/vif.c)
   fills from UNPACK and MPG, and the microprogram, rewritten in C
   (port/src/vu1/, port/docs/vu.md), which MSCAL/MSCALF/MSCNT start. */
#ifndef GRX_VU1_H
#define GRX_VU1_H

#include <stdint.h>

#define GRX_VU1_DATA_SIZE  0x4000 /* 1024 quadwords */
#define GRX_VU1_MICRO_SIZE 0x4000 /* 2048 instruction pairs */

/* VU1 data memory (what UNPACK writes) and micro memory (what MPG
   writes); defined by the VIF1 layer */
extern uint8_t grx_vu1_data[GRX_VU1_DATA_SIZE] __attribute__((aligned(16)));
extern uint8_t grx_vu1_micro[GRX_VU1_MICRO_SIZE] __attribute__((aligned(16)));

/* VIF1's TOPS/TOP/ITOP as the microprogram reads them (xtop, xitop),
   kept by the VIF1 layer */
uint32_t grx_vif1_top(void);
uint32_t grx_vif1_itop(void);

/* Run the microprogram from instruction address addr (MSCAL/MSCALF), or
   continue where it stopped (MSCNT: addr = GRX_VU1_CONTINUE), until its
   E bit. It sends what it XGKICKs to grx_gs_gif(..., GRX_GIF_PATH1).
   Defined by port/src/vu1/. */
#define GRX_VU1_CONTINUE 0xFFFFFFFFu
void grx_vu1_run(uint32_t addr);

#endif
