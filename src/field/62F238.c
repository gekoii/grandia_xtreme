#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    u8 pad24[0x38];
} Work;

extern Work fldMenuMagic;

void *MemSet(void *, s32, s32);

void FldMenuMagicInit(void) {
    MemSet(&fldMenuMagic, 0, 0x5C);
    FldMenuWinOpen(8, 0x140, 0xD4, 0x21C, 0xA4, 0, 0, 0x88000);
    FldMenuWinOpen(0xA, 0x1B0, 0x172, 0x15E, 0x58, 0, 1, 0x80);
    fldMenuMagic.unk20 = -1;
    FldMenuGuideSet(6);
}
