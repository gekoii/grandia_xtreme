#include "common.h"
#include "field/n20.h"
#include "protos/field.h"

void FldMoveCmdGlobalFreeAll(void) {
    FldMoveCmd *n;

    if (fldMoveCmdGlobalList != NULL) {
        n = fldMoveCmdGlobalList->next;
        FldMoveCmdFree(fldMoveCmdGlobalList);
        fldMoveCmdGlobalList = NULL;
        if (n != NULL) {
            do {
                FldMoveCmdFree(n);
                n = n->next;
            } while (n != NULL);
        }
    }
}
