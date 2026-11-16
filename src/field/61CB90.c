#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"

#define DEF(o) ((o)->sense->charDef)

void FldMonsterSetTracks(void *arg0, s32 arg1) {
    ChrObj *o = arg0;
    FldCharDef *d;

    o->tracks[0][0] = ChrTenantTrackCreatePreset(0, DEF(o)->unk18, 0U);
    d = DEF(o);
    o->tracks[0][1] = ChrTenantTrackCreatePreset(1, d->unk18, d->unk14);
    d = DEF(o);
    o->tracks[0][2] = ChrTenantTrackCreatePreset(2, d->unk19, d->unk15);
    o->tracks[0][3] = ChrTenantTrackCreatePreset(3, DEF(o)->unk18, 0U);
    o->tracks[0][4] = ChrTenantTrackCreatePreset(0, 0U, 0U);
    o->tracks[0][5] = ChrTenantTrackCreatePreset(0, 0U, 0U);
}
