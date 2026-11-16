#include "common.h"
#include "protos/main.h"
#include "main/n01.h"


void ChrPortraitSetByCode(u8 arg0, u16 arg1) {
    ChrPortrait **pp;
    u32 i;
    u32 found;

    found = 0xFFFFFFFF;
    for (i = 0; i < chrPortraitIndexTable->count; i++) {
        if (chrPortraitIndexTable->entries[i].code == arg1) {
            found = i;
            break;
        }
    }
    if (found != 0xFFFFFFFF) {
        pp = &chrPortraitSlots.slots[arg0];
        if ((*pp)->cur != found) {
            FileDecompress(chrPortraitSlots.faceOffsets[chrPortraitIndexTable->entries[found].faceIndex], *pp, 1);
            GphTexDataRelocate(*pp);
            GsTexLoadPacket((*pp)->texPacket, *pp, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
            (*pp)->cur = found;
        }
    }
}
