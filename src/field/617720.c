#include "common.h"
#include "grxsys.h"
#include "protos/field.h"
#include "field/state.h"

static inline s32 getId(u32 i, s32 arg1) {
    if (arg1 == 0) {
        return sysGrxsysPac->sndEnv->maps[i].envId;
    } else {
        return sysGrxsysPac->sndEnv->maps[i].envIdAlt;
    }
}

void FldSndApplyMapEnv(u16 key, s32 arg1) {
    u32 i;
    s32 found;
    s32 id;
    s64 id2;
    u32 off;

    found = 0;
    for (i = 0; i < sysGrxsysPac->sndEnv->mapCount; i++) {
        if (sysGrxsysPac->sndEnv->maps[i].mapNo == key) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return;
    }
    id = getId(i, arg1);
    id2 = id;
    if (id == 0) {
        return;
    }
    found = 0;
    for (i = 0; i < sysGrxsysPac->sndEnv->envCount; i++) {
        if (sysGrxsysPac->sndEnv->envs[i].id == id2) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return;
    }
    off = i << 5;
    {
        SysSndEnv *d = (SysSndEnv *)(off + (u32)sysGrxsysPac->sndEnv->envs);
        fldSnd.stepSe[0] = d->vals[0];
        fldSnd.stepSe[1] = d->vals[1];
        fldSnd.stepSe[2] = d->vals[2];
        fldSnd.stepSe[3] = d->vals[3];
        SndSetDistanceScale(fldSnd.distScale = d->distScale);
    }
    SndSetEffect((SysSndEnv *)((u32)sysGrxsysPac->sndEnv->envs + off));
}
