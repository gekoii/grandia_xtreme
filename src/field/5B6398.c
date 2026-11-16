#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

s32 OccCondCheck(OccCond *c) {
    s32 r[2];
    s32 i;
    s32 v;
    u16 want;
    s16 *t;

    if (c->op == 0) {
        return 1;
    }
    if (c->op < 5) {
        for (i = 0; i < 2; i++) {
            t = (s16 *)c + i * 3; /* term i: t[1] type, t[2] id, t[3] value */
            want = t[3];
            v = EvtFlagGetByType(t[1], t[2]);
            if ((want != 0 && v != 0) || (want == 0 && v == 0)) {
                r[i] = 1;
            } else {
                r[i] = 0;
            }
        }
        switch (c->op) {
        case 1:
            return r[0];
        case 2:
            return r[0] & r[1];
        case 3:
            return r[0] | r[1];
        case 4:
            return r[0] ^ r[1];
        default:
            return 0;
        }
    }
    return *evtScnrGameRefs->unk8 == (u8)(c->op - 5);
}
