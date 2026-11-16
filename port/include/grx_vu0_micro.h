/* The game's VU0 microcode (vuMicroCode0) as C: 24 small kernels the CPU
   starts with vcallms, working on the VU0 registers that grx_ee.h keeps
   (grx_vf, grx_vi) and on VU0 data memory (the mapped window at
   0x11004000). See port/docs/vu.md, "VU0: small kernels behind vcallms",
   and port/src/vu0_micro.c.

   vcallms N in translated code becomes grx_vu0_callms(N), with N the
   micro-memory byte address as the disassembly prints it (the instruction
   field times 8). The kernel runs to its end at once; Q, the MAC, status
   and clip flags are left as the VU would have them once it is idle:
   grx_vi[22] (Q, float bits), grx_vi[17] (MAC), grx_vi[16] (status),
   grx_vi[18] (clip), grx_vi[21] (I). */
#ifndef GRX_VU0_MICRO_H
#define GRX_VU0_MICRO_H

void grx_vu0_callms(unsigned int addr);

/* vcallmsr: the address in CMSAR0 (grx_vi[27]), counted in pairs */
void grx_vu0_callmsr(void);

#endif
