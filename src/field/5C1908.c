#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrTenantAiRestart(s32 id, s32 n) {
    ChrObj *a;
    s32 i;

    a = ChrTenantFindById(id);
    if (a != NULL && n > 0) {
        i = n;
        do {
            a->aiMode = 2;
            i--;
            a->opPc = 0;
            a->opTimer = 0;
            a++;
        } while (i != 0);
    }
}
