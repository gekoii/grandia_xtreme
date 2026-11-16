#include "common.h"
#include "field/n14.h"


void ChrTenantTrackPresetWander(ChrTenantTrack *t, u8 type, u8 val) {
    s16 v;

    if (type == 0) {
        type = 1;
    }
    v = val;
    switch (type) {
    case 0:
        t->ops[0].op = 1;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = 0;
        return;
    case 1:
    case 5:
        t->ops[0].op = 3;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 9;
        t->ops[1].arg1 = v;
        t->ops[1].arg2 = 0x3C;
        t->ops[2].op = 9;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 0x3C;
        t->ops[3].op = 7;
        t->ops[3].arg1 = v;
        t->ops[3].arg2 = 0x3C;
        t->ops[4].op = 9;
        t->ops[4].arg1 = v;
        t->ops[4].arg2 = 0x3C;
        t->ops[5].op = 9;
        t->ops[5].arg1 = v;
        t->ops[5].arg2 = 0x3C;
        t->ops[6].op = 7;
        t->ops[6].arg1 = v;
        t->ops[6].arg2 = 0x3C;
        t->ops[7].op = 0x1D;
        t->ops[7].arg1 = 0;
        t->ops[7].arg2 = 0;
        return;
    case 2:
        t->ops[0].op = 3;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 9;
        t->ops[1].arg1 = v;
        t->ops[1].arg2 = 0x3C;
        t->ops[2].op = 9;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 0x3C;
        t->ops[3].op = 2;
        t->ops[3].arg1 = 0;
        t->ops[3].arg2 = 0x3C;
        t->ops[4].op = 7;
        t->ops[4].arg1 = v;
        t->ops[4].arg2 = 0x3C;
        t->ops[5].op = 9;
        t->ops[5].arg1 = v;
        t->ops[5].arg2 = 0x3C;
        t->ops[6].op = 9;
        t->ops[6].arg1 = v;
        t->ops[6].arg2 = 0x3C;
        t->ops[7].op = 2;
        t->ops[7].arg1 = 0;
        t->ops[7].arg2 = 0x3C;
        t->ops[8].op = 0x1D;
        t->ops[8].arg1 = 0;
        t->ops[8].arg2 = 0;
        return;
    case 3:
        t->ops[0].op = 3;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 9;
        t->ops[1].arg1 = v;
        t->ops[1].arg2 = 0x3C;
        t->ops[2].op = 9;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 0x3C;
        t->ops[3].op = 7;
        t->ops[3].arg1 = v;
        t->ops[3].arg2 = 0x3C;
        t->ops[4].op = 2;
        t->ops[4].arg1 = 0;
        t->ops[4].arg2 = 0x3C;
        t->ops[5].op = 0x1D;
        t->ops[5].arg1 = 0;
        t->ops[5].arg2 = 0;
        return;
    case 4:
        t->ops[0].op = 3;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 0xA;
        t->ops[1].arg1 = v;
        t->ops[1].arg2 = 6;
        t->ops[2].op = 0xA;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 6;
        t->ops[3].op = 7;
        t->ops[3].arg1 = v;
        t->ops[3].arg2 = 0x3C;
        t->ops[4].op = 0xA;
        t->ops[4].arg1 = v;
        t->ops[4].arg2 = 6;
        t->ops[5].op = 0xA;
        t->ops[5].arg1 = v;
        t->ops[5].arg2 = 6;
        t->ops[6].op = 7;
        t->ops[6].arg1 = v;
        t->ops[6].arg2 = 0x3C;
        t->ops[7].op = 0x1D;
        t->ops[7].arg1 = 0;
        t->ops[7].arg2 = 0;
        return;
    case 6:
        t->ops[0].op = 3;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 2;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0xF0;
        t->ops[2].op = 9;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 0x3C;
        t->ops[3].op = 2;
        t->ops[3].arg1 = 0;
        t->ops[3].arg2 = 0xF0;
        t->ops[4].op = 0x1D;
        t->ops[4].arg1 = 0;
        t->ops[4].arg2 = 0;
        break;
    default:
        break;
    }
}
