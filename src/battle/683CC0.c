#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n28.h"


void ChrPartDispSetOne(BtlUnit *u, s32 id, s32 flag) {
    void *p;

    if (u->model != NULL) {
        p = ChrModelFindPart(u->model, id);
        if (p != NULL) {
            BtlChrPartsSetHidden(p, flag);
        }
    }
}
