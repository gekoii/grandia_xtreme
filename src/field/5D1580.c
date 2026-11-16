#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0xC];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Obj5D1580;

void ChrTenantDefInitSpot(Obj5D1580 *arg0, u8 arg1) {
    if (arg1 == 1) {
        arg0->unk2 = 3;
    } else {
        arg0->unk2 = 1;
    }
    arg0->unk12 = 0xD2;
    arg0->unk14 = 0x708;
    arg0->unk10 = 0;
    arg0->unk16 = 0;
}
