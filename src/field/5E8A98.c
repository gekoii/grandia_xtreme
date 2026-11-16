#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void GphModelUpdateGsRegs(struct GphModel *);

void ChrPartyCalcPass(void) {
    s32 i;
    ChrObj *p;

    for (i = 0; i < 9; i++) {
        p = ChrGetByNo(i + 1);
        if (p != 0 && (p->flags & 2) && (p->flags & 1) && p->model != 0) {
            GphModelUpdateGsRegs(p->model);
        }
    }
}
