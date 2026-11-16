#include "common.h"
#include "field/n14.h"


void ChrTenantTrackPresetReturn(ChrTenantTrack *t, u8 type, u8 arg2) {
    if (type == 0) {
        type = 1;
    }
    switch (type) {
    case 0:
        t->ops[0].op = 1;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = 0;
        return;
    case 3:
        t->ops[0].op = 5;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = 5;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 1:
    case 2:
    case 4:
    case 6:
        t->ops[0].op = 9;
        t->ops[0].arg1 = 3;
        t->ops[0].arg2 = 0x5A;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    case 5:
        t->ops[0].op = 0x18;
        t->ops[0].arg1 = 0;
        t->ops[0].arg2 = 0;
        t->ops[1].op = 1;
        t->ops[1].arg1 = 0;
        t->ops[1].arg2 = 0;
        return;
    }
}
