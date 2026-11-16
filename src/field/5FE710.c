#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

extern u16 evtCharSelTenantIds[];
extern FVECTOR evtCharSelCam2[];
extern FVECTOR evtCharSelCam3[];
extern FVECTOR evtCharSelCam4[];
extern FVECTOR evtCharSelCam5[];
extern s8 evtCharSelPlace2[][2];
extern s8 evtCharSelPlace3[][3];
extern s8 evtCharSelPlace4[][4];

void MemSet(void *, s32, s32);
void VuVecLerp(FVECTOR *, FVECTOR *, FVECTOR *, f32);

#define QCOPY(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void EvtCharSelStart(s32 arg0, s32 arg1) {
    s32 i;
    u16 *tbl;
    ChrObj *obj;
    GphPart *m;
    s32 r;

    if (arg0 == 0) {
        evtCharSelWork->state = 4;
        return;
    }
    MemSet(evtCharSelWork, 0, 0x180);
    evtCharSelWork->mode = arg0;
    evtCharSelWork->state = 1;
    tbl = evtCharSelTenantIds;
    evtCharSelWork->lookT = 0.0f;
    evtCharSelWork->count = 0;
    for (i = 0; i < 9; i++, tbl++) {
        if (EvtSystemFlagGet(i + 0x29) == 0) {
            continue;
        }
        evtCharSelWork->chars[evtCharSelWork->count] = EvtCharSelFindTenant(*tbl);
        evtCharSelWork->flagIds[evtCharSelWork->count] = i + 0x33;
        obj = evtCharSelWork->chars[evtCharSelWork->count];
        if (obj == NULL) {
            continue;
        }
        obj->mot = 3;
        obj->flags |= 0x43;
        m = ChrModelFindPart(evtCharSelWork->chars[evtCharSelWork->count]->model, 0x366);
        if (m != NULL) {
            m->color[3] = 0.0f;
            m->flags |= 8;
        }
        m = ChrModelFindPart(evtCharSelWork->chars[evtCharSelWork->count]->model, 0x302);
        if (m != NULL) {
            m->color[3] = 0.0f;
            m->flags |= 8;
        }
        m = ChrModelFindPart(evtCharSelWork->chars[evtCharSelWork->count]->model, 0x30C);
        if (m != NULL) {
            m->color[3] = 0.0f;
            m->flags |= 8;
        }
        m = ChrModelFindPart(evtCharSelWork->chars[evtCharSelWork->count]->model, 0x280);
        if (m != NULL) {
            m->flags = 0;
        }
        ChrMotSetRandomFrame(evtCharSelWork->chars[evtCharSelWork->count], evtCharSelWork->chars[evtCharSelWork->count]->mot);
        ChrMotFlagsSet(evtCharSelWork->chars[evtCharSelWork->count]->model, 2, 0);
        ChrMotFlagsSet(evtCharSelWork->chars[evtCharSelWork->count]->model, 4, 1);
        evtCharSelWork->idleCount[evtCharSelWork->count] = VuRandInt(4);
        if (++evtCharSelWork->count >= 5) {
            break;
        }
    }
    evtCharSelWork->cursor = (GphModel *)MapModelFindById(0xC9);
    evtCharSelWork->cursor2 = (GphModel *)MapModelFindById(0xC8);
    switch (evtCharSelWork->count) {
    case 2:
        r = VuRandInt(7);
        for (i = 0; i < 2; i++) {
            EvtCharSelPlace(i, evtCharSelPlace2[r][i]);
        }
        QCOPY(&evtCharSelWork->eyeKey, &evtCharSelCam2[r]);
        VuVecLerp(&evtCharSelWork->lookKeys[0], &evtCharSelWork->chars[0]->model->pos, &evtCharSelWork->chars[1]->model->pos, 0.45f);
        VuVecLerp(&evtCharSelWork->lookKeys[1], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[0]->model->pos, 0.45f);
        break;
    case 3:
        r = VuRandInt(3);
        for (i = 0; i < 3; i++) {
            EvtCharSelPlace(i, evtCharSelPlace3[r][i]);
        }
        QCOPY(&evtCharSelWork->eyeKey, &evtCharSelCam3[r]);
        VuVecLerp(&evtCharSelWork->lookKeys[0], &evtCharSelWork->chars[0]->model->pos, &evtCharSelWork->chars[1]->model->pos, 0.75f);
        QCOPY(&evtCharSelWork->lookKeys[1], &evtCharSelWork->chars[1]->model->pos);
        VuVecLerp(&evtCharSelWork->lookKeys[2], &evtCharSelWork->chars[2]->model->pos, &evtCharSelWork->chars[1]->model->pos, 0.75f);
        break;
    case 4:
        r = VuRandInt(2);
        for (i = 0; i < 4; i++) {
            EvtCharSelPlace(i, evtCharSelPlace4[r][i]);
        }
        QCOPY(&evtCharSelWork->eyeKey, &evtCharSelCam4[r]);
        VuVecLerp(&evtCharSelWork->lookKeys[0], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[2]->model->pos, 0.25f);
        VuVecLerp(&evtCharSelWork->lookKeys[1], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[2]->model->pos, 0.45f);
        VuVecLerp(&evtCharSelWork->lookKeys[2], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[2]->model->pos, 0.65f);
        VuVecLerp(&evtCharSelWork->lookKeys[3], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[2]->model->pos, 0.85f);
        break;
    case 5:
        r = VuRandInt(3);
        for (i = 0; i < 5; i++) {
            EvtCharSelPlace(i, i);
        }
        QCOPY(&evtCharSelWork->lookKeys[0], &evtCharSelWork->chars[0]->model->pos);
        QCOPY(&evtCharSelWork->eyeKey, &evtCharSelCam5[r]);
        VuVecLerp(&evtCharSelWork->lookKeys[0], &evtCharSelWork->chars[2]->model->pos, &evtCharSelWork->chars[1]->model->pos, 0.5f);
        VuVecLerp(&evtCharSelWork->lookKeys[1], &evtCharSelWork->chars[2]->model->pos, &evtCharSelWork->chars[1]->model->pos, 0.25f);
        VuVecLerp(&evtCharSelWork->lookKeys[2], &evtCharSelWork->chars[1]->model->pos, &evtCharSelWork->chars[2]->model->pos, 1.0f);
        VuVecLerp(&evtCharSelWork->lookKeys[3], &evtCharSelWork->chars[2]->model->pos, &evtCharSelWork->chars[3]->model->pos, 0.25f);
        VuVecLerp(&evtCharSelWork->lookKeys[4], &evtCharSelWork->chars[2]->model->pos, &evtCharSelWork->chars[3]->model->pos, 0.5f);
        break;
    }
}
