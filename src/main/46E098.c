/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u32 chcr;
    u32 pad04[3];
    u32 madr;
    u32 pad14[7];
    u32 tadr;
    u32 pad34[3];
    u32 asr1;
    u32 pad44[3];
    u32 asr0;
    u32 pad54[11];
    u32 sadr;
} DmaCh;

extern volatile DmaCh *_sceDmaChanTable[10];
extern s32 _sceDmaChanHasTag[10];

#define REG(a) (*(volatile u32 *)(a))

s32 sceDmaReset(s32 arg0) {
    u8 buf[0x20];
    s32 i;
    s32 enabled;

    enabled = REG(0x1000E000) & 1;
    for (i = 0; i < 10; i++) {
        if (_sceDmaChanHasTag[i] != 0) {
            _sceDmaChanTable[i]->sadr = 0;
            _sceDmaChanTable[i]->chcr = 0;
            _sceDmaChanTable[i]->tadr = 0;
            _sceDmaChanTable[i]->madr = 0;
            _sceDmaChanTable[i]->asr0 = 0;
            _sceDmaChanTable[i]->asr1 = 0;
        }
    }
    REG(0x1000E010) = 0xFF1F;
    REG(0x1000E010) &= 0xFF1F0000;
    _sceDmaBzero(buf, 0x14);
    sceDmaPutEnv(buf);
    if (arg0 == 1) {
        REG(0x1000E000) |= 1;
    }
    return enabled;
}
