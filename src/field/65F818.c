#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadCheckCard(void) {
    McPortState *st;
    s32 n;

    st = McGetCurPort();
    n = st->state;
    if (n == 2) {
        if (st->type != n) {
            McLoadSetErrorMsg(mcLoadNotPs2Str);
            ((s32 (*)(s32))McLoadSetState)(0x309);
        } else if (st->formatted == 0) {
            ((s32 (*)(s32))McLoadSetState)(0x308);
        } else {
            ((s32 (*)(s32))McLoadSetState)(0x101);
        }
    }
}
