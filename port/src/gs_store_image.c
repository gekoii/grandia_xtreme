/* sceGsExecStoreImage (libgraph, src/main/46D9A8.c), for the port: a read
   of GS local memory back into main memory. On the PS2 it sends a 7-qword
   packet (BITBLTBUF, TRXPOS, TRXREG, FINISH, TRXDIR = 1) through VIF1,
   turns the bus round (BUSDIR) and DMAs the pixels in from the VIF1 FIFO.
   Here the packet goes through the port's VIF1 (port/src/vif.c) and the
   pixels come from grx_gs_read_image. The sizes follow the original: the
   whole quadwords first, then a partial one whose extra bytes, and the
   padding the GS sends after it, are dropped. Defines a game name, so no
   <windows.h>. */
#include <stdint.h>
#include <string.h>
#include "grx_gs.h"
#include "grx_vif.h"
#include "grx_dma.h"

uint64_t GsGetIMR(void);
uint64_t GsPutIMR(uint64_t imr);

/* The parts of the game's GsStoreImage it reads (one qword each: data,
   then the register's address) */
#define SI_BITBLTBUF 0x20
#define SI_TRXREG 0x40
#define SI_QWORDS 7

/* Scratchpad addresses are mapped where they are; uncached aliases are
   the plain address */
static void *ee_dst(void *p) {
    uintptr_t a = (uintptr_t)p;
    uintptr_t seg = a & 0x70000000u;
    if (seg == 0x20000000u || seg == 0x30000000u) {
        a &= 0x0FFFFFFFu;
    }
    return (void *)a;
}

int32_t sceGsExecStoreImage(void *sp, void *dstaddr) {
    uint8_t *s = sp;
    uint64_t bitbltbuf, trxreg, imr;
    int32_t w, h, h2 = 0, psm, bytes = 0, odd, rest, qwc, pad = 0, size;
    uint8_t *dst = ee_dst(dstaddr);

    memcpy(&bitbltbuf, s + SI_BITBLTBUF, 8);
    memcpy(&trxreg, s + SI_TRXREG, 8);
    w = (int32_t)(trxreg & 0xFFF);
    h = (int32_t)((trxreg >> 32) & 0xFFF);
    psm = (int32_t)((bitbltbuf >> 24) & 0x3F);

    /* Bytes per pixel by format, and the height the GS rounds a partial
       last quadword up to (in rows), as the original works them out */
    switch (psm) {
    case 0x00: case 0x30: bytes = w * h * 4; break;
    case 0x01: case 0x31: bytes = w * h * 3; break;
    case 0x02: case 0x0A: case 0x32: case 0x3A: bytes = w * h * 2; break;
    case 0x13: case 0x1B: bytes = w * h; break;
    case 0x14: case 0x24: case 0x2C: bytes = (w * h) >> 1; break;
    default: break;
    }
    odd = bytes & 0xF;
    rest = (bytes >> 4) & 7;
    qwc = (bytes >> 4) & ~7;
    if (odd) {
        switch (psm) {
        case 0x00: case 0x30: h2 = (h + 3) & 0x1FFC; size = (w * h2) >> 2; break;
        case 0x01: case 0x31: h2 = (h + 15) & 0x1FF0; size = (w * h2 * 3) >> 4; break;
        case 0x02: case 0x0A: case 0x32: case 0x3A: h2 = (h + 7) & ~7; size = (w * h2) >> 3; break;
        case 0x13: case 0x1B: h2 = (h + 7) & ~7; size = (w * h2) >> 4; break;
        default: h2 = (h + 7) & ~7; size = (w * h2) >> 5; break;
        }
        pad = size - qwc - rest - 1;
        /* The GS is asked for whole rows up to h2 */
        trxreg = (uint64_t)(uint32_t)w | ((uint64_t)(uint32_t)h2 << 32);
        memcpy(s + SI_TRXREG, &trxreg, 8);
    }

    imr = GsPutIMR(GsGetIMR() | 0x200);
    grx_gs_csr_write(2); /* clear FINISH */
    grx_vif_write(1, (const uint32_t *)s, SI_QWORDS * 4);

    if (qwc + rest > 0) {
        grx_gs_read_image(dst, (uint32_t)(qwc + rest));
    }
    if (odd) {
        uint8_t last[16];
        int32_t i;
        grx_gs_read_image(last, 1);
        memcpy(dst + (size_t)(qwc + rest) * 16, last, (size_t)odd);
        for (i = 0; i < pad; i++) {
            grx_gs_read_image(last, 1);
        }
    }

    GsPutIMR(imr);
    grx_gs_csr_write(2);
    return 0;
}
