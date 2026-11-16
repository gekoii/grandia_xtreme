#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void MemSet(void *, s32, s32);
void ChrTenantDefInitSpot(void *, u8);

ChrObj *FldMonsterSpawn(s32 arg0, s16 arg1) {
    u8 buf[0xC0];
    s32 id;
    ChrObj *o;
    s32 i;

    id = ChrTenantGetUsed();
    MemSet(buf, 0, 0xC0);
    ChrTenantDefInitSpot(buf, arg1);
    o = ChrTenantAdd(buf);
    FldMonsterSetTracks(o, 0);
    if (arg1 == 0) {
        for (i = 7; i >= 0; i--) {
            o->areas[i] = NULL;
        }
        EvtMapFlagSet(id, 0);
    } else if (EvtMapFlagGet(id) == 0) {
        ChrTenantSetActive(o, 1);
        o->flags |= 1;
    }
    return o;
}
