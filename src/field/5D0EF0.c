#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n14.h"
#include "field/n15.h"

s32 MapPathBuild(FldPath *);

void CamScrPathStart(CamScrPath *arg0, s16 arg1) {
    s32 i;
    s32 found;
    s32 r;

    found = -1;
    for (i = 0; i < fldPathCount; i++) {
        if (fldPathTable[i].id == arg1) {
            found = i;
            break;
        }
    }
    if (found < 0 || (r = MapPathBuild(&fldPathTable[found])) < 0) {
        arg0->active = 0;
        return;
    }
    if (r & 0x10000) {
        arg0->type = 1;
        r &= 0xFF;
        arg0->idx = r;
        arg0->count = mapPathWork->bigUsed[r];
    } else {
        arg0->type = 0;
        r &= 0xFF;
        arg0->idx = r;
        arg0->count = mapPathWork->smallUsed[r];
    }
    arg0->frame = 0;
    arg0->active = 1;
}
