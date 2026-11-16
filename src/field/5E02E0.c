#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void GsTexUpload(void *);

#define SETUP(env, tex) \
    GsRenderStatePacketSend(effMapWork->drawEnvs[env]); \
    GsTexUpload(&effMapWork->texObjs[tex]); \
    GsTexPacketSend(&effMapWork->texObjs[tex])

void EffDraw(void) {
    s32 i;

    if (effMapWork->fireCount != 0) {
        SETUP(1, 3);
        for (i = 0; i < effMapWork->fireCount; i++) {
            EffObjDraw(effMapWork->fireList[i]);
        }
    }
    if (effMapWork->driftCount != 0) {
        SETUP(1, 6);
        for (i = 0; i < effMapWork->driftCount; i++) {
            EffObjDraw(effMapWork->fireList[i]);
        }
    }
    if (effMapWork->charaCount != 0) {
        SETUP(1, 3);
        for (i = 0; i < effMapWork->charaCount; i++) {
            EffObjDraw(effMapWork->charaPool[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->pool16, 16)) {
        SETUP(0, 3);
        for (i = 0; i < 16; i++) {
            EffObjDraw(effMapWork->pool16[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->pool5A, 4)) {
        SETUP(1, 7);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->pool5A[i]);
        }
    }
    if (effMapWork->moteCount != 0) {
        SETUP(1, 7);
        for (i = 0; i < effMapWork->moteCount; i++) {
            EffObjDraw(effMapWork->moteList[i]);
        }
    }
    if (effMapWork->steamCount != 0) {
        if ((EvtGetMapNo() & 0xFF00) == 0x4800) {
            GsRenderStatePacketSend(effMapWork->drawEnvs[1]);
        } else {
            GsRenderStatePacketSend(effMapWork->drawEnvs[0]);
        }
        GsTexUpload(&effMapWork->texObjs[3]);
        GsTexPacketSend(&effMapWork->texObjs[3]);
        for (i = 0; i < effMapWork->steamCount; i++) {
            EffObjDraw(effMapWork->steamList[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->spotOpenPool, 4)) {
        SETUP(0, 3);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->spotOpenPool[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->spotPool, 4)) {
        SETUP(2, 3);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->spotPool[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->poolE0, 4)) {
        SETUP(0, 2);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->poolE0[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->pickupPool, 4)) {
        SETUP(1, 6);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->pickupPool[i]);
        }
    }
    if (effMapWork->ringCount != 0) {
        SETUP(1, 6);
        for (i = 0; i < effMapWork->ringCount; i++) {
            EffObjDraw(effMapWork->ringList[i]);
        }
    }
    if (EffListAnyVisible(effMapWork->charaPool, 4)) {
        SETUP(1, 6);
        for (i = 0; i < 4; i++) {
            EffObjDraw(effMapWork->charaPool[i]);
        }
    }
}
