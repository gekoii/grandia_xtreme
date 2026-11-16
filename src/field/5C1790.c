#include "common.h"
#include "protos/field.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void *ChrTenantTrackCreatePreset(u8 type, u8 a, u8 b) {
    void *t = FldMemAlloc(0xC6);
    MemSet(t, 0, 0xC6);
    switch (type) {
    case 0:
        ChrTenantTrackPresetNone(t, a, b);
        break;
    case 1:
        ChrTenantTrackPresetWander(t, a, b);
        break;
    case 2:
        ChrTenantTrackPresetReact(t, a, b);
        break;
    case 3:
        ChrTenantTrackPresetReturn(t, a, b);
        break;
    }
    return t;
}
