#include "common.h"
#include "protos/field.h"


u8 *EvtMsgSkipLeadOps(u8 *p) {
    while ((*p >= 8 && *p <= 11) || *p == 3) {
        if (*p == 3) {
            p = EvtScnrOpJump(p + 1);
        } else if (*p == 8) {
            p = EvtScnrOpVar(p + 1, 0);
        } else if (*p == 9) {
            p = EvtScnrOpVar(p + 1, 1);
        } else if (*p == 10) {
            p = EvtScnrOpVar(p + 1, 2);
        } else if (*p == 11) {
            p = EvtScnrOpFlag(p + 1);
        }
    }
    return p;
}
