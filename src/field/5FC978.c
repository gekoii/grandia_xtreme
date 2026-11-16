#include "common.h"
#include "protos/field.h"
#include "field/event.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Ent;


void EvtTenantPoseRequestFlush(void) {
    Ent *e;
    s32 i;
    s32 r;

    e = EvtScnrGetTenantReqs();
    if (EvtScnrGetWork()->unk46C != 0) {
        for (i = 0; i < EvtScnrGetWork()->unk46C; i++) {
            r = ChrTenantFindById(e->unk0);
            if (r != 0) {
                ChrTenantSetArea(r, e->unk4);
            }
            e++;
        }
        EvtScnrGetWork()->unk46C = 0;
    }
}
