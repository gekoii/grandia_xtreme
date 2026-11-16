/*
 * The EE side of SIF, and the IOP behind it (port/src/sif.c).
 *
 * The port replaces Sony's SIF libraries (sifcmd, sifrpc, loadfile and the
 * IOP reset calls, src/main/48E078.c-493CB8.c) at the sceSif* level. There
 * is no IOP: modules "load" by name, RPC calls are answered at once by C
 * handlers standing in for the IOP modules' servers, and SIF DMAs go to
 * whatever stands in for the IOP memory they address. Everything is
 * synchronous: a call or DMA is finished when it returns.
 *
 * Other parts of the platform layer plug into it here: an IOP module's
 * RPC server (pad, memory card), or a range of IOP addresses that EE DMAs
 * go to (libpad sends its actuator buffers to the address padman gave it).
 *
 * Only for the port's own sources (no game headers needed).
 */
#ifndef GRX_SIF_H
#define GRX_SIF_H

#include <stdint.h>

/* One RPC call to an IOP server: (fno, send, ssize, recv, rsize) as the EE
   passed them to sceSifCallRpc. The handler reads `send` and writes at
   most `rsize` bytes of reply to `recv` (either may be NULL / 0). Returns
   0, or -1 when it doesn't know the call (the call then stops the game
   with the server's name and fno). */
typedef int (*GrxSifRpcHandler)(uint32_t fno, void *send, int32_t ssize, void *recv, int32_t rsize);

/* Sets the handler for an IOP RPC server id (0x80000100 padman, ...).
   The server exists only once the module that provides it is loaded
   (sceSifLoadModule), as on the PS2. A server with no handler binds, and
   its first call stops the game: "not emulated". */
void grx_sif_set_rpc_handler(uint32_t sid, GrxSifRpcHandler fn);

/* EE-to-IOP DMA (sceSifSetDma) into [lo, hi): fn gets each descriptor in
   the range. Returns nonzero when it took the data. */
typedef int (*GrxSifDmaHandler)(uint32_t iop_addr, const void *src, uint32_t size);
void grx_sif_add_dma_target(uint32_t lo, uint32_t hi, GrxSifDmaHandler fn);

/* Whether a module was loaded since the last IOP reboot ("PADMAN.IRX",
   matched without directory, version or case) */
int grx_sif_module_loaded(const char *name);

/* Scratch for SndFsMount's directory reads, in place of EE 0x01000000
   (below 0x08000000, for SndFsReadDir's & 0x7FFFF80) */
void *grx_sif_dir_scratch(void);

/* The EE/IOP shared block (cdvd.c), PS2 0x01FF0000-0x02000000: GS_IOP's
   header at +0, the status copy at +0x100, the directory tree at +0x1000.
   A plain object below 0x08000000, not memory at the PS2 address: the
   game's literal uses are rewritten to it (port/tools/fixups/sif.py). */
#define GRX_IOP_BLOCK_PS2  0x01FF0000u
#define GRX_IOP_BLOCK_SIZE 0x00010000u
void *grx_iop_block(void);
/* A PS2 address in the block, or in its 0x2/0x3 uncached aliases, as a
   host address; any other address unchanged */
uintptr_t grx_iop_addr(uint32_t ps2);

#endif
