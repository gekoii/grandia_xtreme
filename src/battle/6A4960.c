#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"


void FxComboLinkSet(BtlUnit *arg0, u8 arg1) {
    FxComboLink *d = &fxComboLink;
    FxComboLinkPart *e;
    FxComboLinkPart *t;
    s32 i;

    if (arg1 == 0) {
        if (fxComboLink.parts != NULL) {
            d->ending = 1;
        }
    } else {
        if (fxComboLink.parts != NULL) {
            MemFree(fxComboLink.parts);
        }
        t = (FxComboLinkPart *)BtlAlloc32(0x2D0);
        fxComboLink.parts = t;
        if (t != NULL) {
            d->members = arg1;
            d->next = 0;
            d->ending = 0;
            d->actor = arg0;
            d->radius = arg0->params->colR * 0.5f;
            e = t;
            i = 0x5A;
            do {
                e->life = 0;
                i--;
                e++;
            } while (i > 0);
        }
    }
}
