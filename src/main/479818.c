/* rodata: 0x004A7700-0x004A7720 */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"

void SyncDCache(void *, void *);

void _scePadSendActDirect(s32 port, s32 slot) {
    sceSifDmaData dma[16];
    s32 *buf;
    s32 id;
    u8 *dst;

    buf = &_scePadPortTable[port][slot].actBuf->count;
    if (sceSifDmaStat(_scePadPortTable[port][slot].dmaId) >= 0) {
        if (_scePadDebugPrint != 0) {
            printf("libpad: sceSifSetDma faild\n");
        }
        return;
    }
    (*buf)++;
    dst = _scePadPortTable[port][slot].iopAddr + ((*buf & 1) << 5);
    SyncDCache(buf, (u8 *)buf + 0x20);
    dma[0].data = (u32)buf;
    dma[0].addr = (u32)dst;
    dma[0].size = 0x20;
    dma[0].mode = 0;
    id = sceSifSetDma(dma, 1);
    if (id == 0) {
        if (_scePadDebugPrint != 0) {
            printf("libpad: sceSifSetDma faild\n");
        }
    }
    _scePadPortTable[port][slot].dmaId = id;
}
