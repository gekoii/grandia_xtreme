#include "common.h"
#include "field/n14.h"

void ChrMotTableSet(ChrObj *o, s32 val, s32 idx) {
    ChrMotTable *p;

    if (idx < 64) {
        p = o->motTable;
        if (p != NULL && val != 0) {
            p->count++;
            p->slots[idx] = val;
        }
    }
}
