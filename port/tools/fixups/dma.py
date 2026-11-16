"""DMA, done synchronously (port/src/dma.c, port/include/grx_dma.h,
port/docs/hardware.md (b) 3).

The SDK's libdma and the game's MFIFO start stay as they are: they set the
channel's registers in the mapped page and store CHCR with STR, and each of
those stores is followed by grx_dma_kick(), which runs the whole transfer
and writes the registers back as the DMAC leaves them. Every poll on CHCR,
QWC or MADR elsewhere then sees a finished transfer and needs no change.

The declarations go on the line after an #include, and the calls on the
line of the store, so the generated copies keep their line numbers.

Left alone, on purpose:
- sceDmaWatch (46E9D8), sceDmaSync (46E910), sceDmaPause (46EAA8),
  sceDmaPutEnv, sceDmaPutStallAddr: they only read or set state.
  sceDmaWatch's timeout path clears STR, which is harmless.
- GsMfifoStop (455EE8): it clears STR on fromSPR and VIF1, and the platform
  keeps no state of its own for the MFIFO beyond the registers.
- _sceWaitVSync/_sceWaitVSyncCsr are port/src/video.c's (frame.py).
"""

KICK_DECL = "void grx_dma_kick(volatile void *); "
CSR_DECL = "void grx_gs_csr_write(unsigned long long); "

FIXUPS = {
    # sceDmaReset: the platform hears of it (its D_STAT write-1-to-clear
    # stores already leave the plain memory 0, which is what they mean)
    "src/main/46E098.c": [
        (r'(#include "protos/main.h"\n)', r"\1void grx_dma_reset(void); "),
        (r"(    REG\(0x1000E010\) &= 0xFF1F0000;)", r"\1 grx_dma_reset();"),
    ],
    # sceDmaSend (chain) and sceDmaSendN (normal): the CHCR store with STR
    "src/main/46E3C0.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(        d->chcr = c;)(\n    \}\n\}\n)", r"\1 grx_dma_kick(d);\2"),
    ],
    "src/main/46E498.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(        d->chcr = c;)(\n    \}\n\}\n)", r"\1 grx_dma_kick(d);\2"),
    ],
    # sceDmaSendI (interleave), sceDmaRecv, sceDmaRecvN, sceDmaRecvI: the
    # last bit-field store sets STR
    "src/main/46E580.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(    arg0->u\.b\.lock = 1;)(\n\}\n)", r"\1 grx_dma_kick(arg0);\2"),
    ],
    "src/main/46E668.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(    arg0->u\.b\.lock = 1;)(\n\}\n)", r"\1 grx_dma_kick(arg0);\2"),
    ],
    "src/main/46E730.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(    arg0->f0\.b\.b8 = 1;)(\n\}\n)", r"\1 grx_dma_kick(arg0);\2"),
    ],
    "src/main/46E820.c": [
        (r'(#include "protos/main.h"\n)', r"\1" + KICK_DECL),
        (r"(    ch->flags = \(\(\(s32\)ch->flags & ~0xC \| 8\) & ~1\) \| 0x100;)(\n\}\n)",
         r"\1 grx_dma_kick(ch);\2"),
    ],
    # sceDmaRestart: stores a saved CHCR back (uncalled)
    "src/main/46EAF0.c": [
        (r'(#include "m2c_macros.h"\n)', r"\1" + KICK_DECL),
        (r"(    \*arg0 = arg1;)", r"\1 grx_dma_kick(arg0);"),
    ],
    # GsMfifoStart: VIF1 started in chain mode on the empty ring. It stays
    # started; each fromSPR send into the ring then drains it at once.
    "src/main/455E08.c": [
        (r'(#include "main/n08.h"\n)', r"\1" + KICK_DECL),
        (r"(    b->chcr = x;)", r"\1 grx_dma_kick(b);"),
    ],
    # GS CSR stores (hw_survey.py --sites: the only ones in the game's C):
    # the real CSR's semantics through grx_gs_csr_write (dma.c). Its event
    # bits are write-1-to-clear and RESET resets the GS, where a plain
    # store would set them. sceGsResetGraph: RESET (mode 0), FLUSH (1).
    "src/main/46CF80.c": [
        (r'(#include "common\.h"\n)', r"\1" + CSR_DECL),
        (r"\*\(volatile u64 \*\)0x12001000 = (0x200|0x100);", r"grx_gs_csr_write(\1);"),
    ],
    # sceGsExecStoreImage: FINISH cleared (2), FLUSH (0x100). The read-back
    # itself (VIF1 to memory) isn't supported yet; it has one caller, a
    # frame-buffer capture (m:444888).
    "src/main/46D9A8.c": [
        (r'(#include "protos/main\.h"\n)', r"\1" + CSR_DECL),
        (r"\*GS_CSR = (2|0x100);", r"grx_gs_csr_write(\1);"),
    ],
    # _sceSioPutRawChar: the SIO's TX FIFO (its full bit never sets in plain
    # memory) to stderr
    "src/main/48D700.c": [
        (r'(#include "common\.h"\n)', r"\1void grx_sio_putc(int); "),
        (r"\*\(volatile u8 \*\)0x1000F180 = c;", "grx_sio_putc(c);"),
    ],
}
