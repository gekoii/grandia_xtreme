#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"
#include "field/n16.h"

void *FldMemAlloc(s32);
void VuSinCosRad(f32 *, f32 *, f32);
void VuVecCopy(VuVec4 *, VuVec4 *);

void EffSpawnCode28(VuVec4 *pos) {
    VuVec4 v;
    f32 s, c;
    s32 i;

    switch (EvtGetMapNo()) {
    case 0x4800:
    case 0x4820:
    case 0xD8A6:
        for (i = 0; i < 44; i++) {
            VuSinCosRad(&s, &c, i * 7.5f);
            VuVecCopy(&v, pos);
            v.x += c * 3000.0f;
            v.z += s * 3000.0f;
            effMapWork->ringList[i] = FldMemAlloc(0x12D0);
            effMapWork->ringList[i]->unk12CC = FldMemAlloc(0x200);
            PtclPresetFallingMotes(effMapWork->ringList[i], &v);
        }
        effMapWork->ringCount = 20;
        break;
    case 0x4810:
    case 0x48F0:
        for (i = 0; i < 8; i++) {
            VuVecCopy(&v, pos);
            v.x += VuRandSigned(3000.0f);
            v.z += VuRandSigned(2000.0f);
            effMapWork->ringList[i] = FldMemAlloc(0x12D0);
            effMapWork->ringList[i]->unk12CC = FldMemAlloc(0x300);
            PtclPresetEmberField(effMapWork->ringList[i], &v);
        }
        effMapWork->ringCount = 8;
        break;
    default:
        for (i = 0; i < 44; i++) {
            VuVecCopy(&v, pos);
            v.x += VuRandSigned(3000.0f);
            v.y -= __builtin_fabsf(VuRandSigned(6000.0f));
            v.z += VuRandSigned(6000.0f);
            effMapWork->ringList[i] = FldMemAlloc(0x12D0);
            effMapWork->ringList[i]->unk12CC = FldMemAlloc(0x200);
            PtclPresetFallingMotes(effMapWork->ringList[i], &v);
        }
        effMapWork->ringCount = 44;
        break;
    }
}
