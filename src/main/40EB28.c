#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n01.h"


void ChrPortraitSet(u8 arg0, s32 arg1) {
    ChrPortrait **pp;

    pp = &chrPortraitSlots.slots[arg0];
    if ((*pp)->cur != arg1) {
        if (ChrPortraitExists(arg1) != 0) {
            FileDecompress(chrPortraitSlots.faceOffsets[chrPortraitIndexTable->entries[arg1].faceIndex], *pp, 1);
            GphTexDataRelocate(*pp);
            GsTexLoadPacket((*pp)->texPacket, *pp, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
            (*pp)->cur = arg1;
        }
    }
}
