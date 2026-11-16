#include "common.h"
#include "protos/main.h"
#include "main/n01.h"


void ChrPortraitSetByIndex(u8 arg0, s32 arg1) {
    ChrPortrait **pp;

    pp = &chrPortraitSlots.slots[arg0];
    if ((*pp)->cur != arg1) {
        FileDecompress(chrPortraitSlots.faceOffsets[arg1], *pp, 1);
        GphTexDataRelocate(*pp);
        GsTexLoadPacket((*pp)->texPacket, *pp, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
        (*pp)->cur = arg1;
    }
}
