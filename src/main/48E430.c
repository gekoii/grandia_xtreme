/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"

extern void sceSifWriteBackDCache(void *, s32);

u32 _sceSifSendCmd(s32 cid, s32 mode, sceSifCmdHdr *pkt, s32 pktsize, void *src, u32 dest, s32 size) {
    sceSifDmaData dma[2];
    s32 n;

    if ((u32)(pktsize - 0x10) > 0x60) {
        return 0;
    }
    n = 0;
    if (size > 0) {
        dma[0].data = (u32)src;
        dma[0].addr = dest;
        dma[0].size = size;
        dma[0].mode = 0;
        pkt->dsize = size;
        pkt->dest = dest;
        n = 1;
        if (mode & 4) {
            sceSifWriteBackDCache(src, size);
        }
    } else {
        pkt->dsize = 0;
        pkt->dest = 0;
    }
    dma[n].data = (u32)pkt;
    dma[n].addr = _sceSifCmdDataIopBuf;
    dma[n].size = pktsize;
    pkt->fcode = cid;
    pkt->psize = pktsize;
    dma[n].mode = 0x44;
    n++;
    sceSifWriteBackDCache(pkt, pktsize);
    if (mode & 1) {
        return isceSifSetDma(dma, n);
    }
    return sceSifSetDma(dma, n);
}
