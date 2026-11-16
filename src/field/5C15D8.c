#include "common.h"
#include "field/n14.h"


void ChrTenantTrackPresetReact(ChrTenantTrack *t, u8 type, u8 val) {
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
        t->ops[0].op = 0xB;
        t->ops[0].arg1 = v;
        t->ops[0].arg2 = 0x78;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 2:
        t->ops[0].op = 6;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 3:
        t->ops[0].op = 0xC;
        t->ops[0].arg1 = v;
        t->ops[0].arg2 = 0x78;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 4:
        t->ops[0].op = 8;
        t->ops[0].arg1 = v;
        t->ops[0].arg2 = 0xA;
        t->ops[1].op = 8;
        t->ops[1].arg1 = v;
        t->ops[1].arg2 = 0xA;
        t->ops[2].op = 8;
        t->ops[2].arg1 = v;
        t->ops[2].arg2 = 0xA;
        t->ops[3].op = 1;
        t->ops[3].arg1 = 0;
        t->ops[3].arg2 = 0;
        return;
    case 11:
        t->ops[0].op = 0x19;
        t->ops[0].arg1 = v;
        t->ops[0].arg2 = 0x78;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 12:
        t->ops[0].op = 0x17;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = v;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    }
}
