#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


/* Calls through an s32-returning pointer keep these from becoming tail calls. */
#define SIB(f) ((s32 (*)())(f))

void OccSwitchForward(OccEntry *obj) {
    OccSwitch *s = OCC_PARAMS(obj, OccSwitch);
    s32 i;
    GphModel *c;
    ChrObj *m;
    GphPart *e;

    switch (s->mode) {
    case 0:
        switch (OCC_PARAMS(obj, OccSwitch)->target) {
        case 0:
            if (obj->flags & OCC_PART) {
                c = FldObjGet(s->id);
                if (c != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(c, i);
                        if (e != NULL) {
                            e->flags |= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 0);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i <= s->last; i++) {
                    GphModel *c = FldObjGet(s->id + i);
                    if (c != NULL) {
                        c->flags |= 8;
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i <= s->last; i++) {
                EvtArea *h = EvtAreaGet(s->id + i);
                if (h != NULL) {
                    EvtAreaDisable(h);
                }
            }
            break;
        case 2:
            for (i = 0; i <= s->last; i++) {
                FldSndEmitterEnable(s->id + i, 0);
            }
            break;
        case 3:
            for (i = 0; i <= s->last; i++) {
                FldPointLightSetEnable(s->id + i, 0);
            }
            break;
        case 4:
            for (i = 0; i <= s->last; i++) {
                OccStopLoop(s->id + i);
            }
            break;
        case 5:
            for (i = 0; i <= s->last; i++) {
                OccsStopLoop(s->id + i);
            }
            break;
        case 6:
            if (obj->flags & OCC_PART) {
                m = ChrTenantGet(s->id);
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags |= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 0);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    m = ChrTenantGet(s->id + i);
                    if (m != NULL) {
                        m->flags = (s16)m->flags & ~1;
                    }
                }
            }
            break;
        case 7:
        case 18:
            if (obj->flags & OCC_PART) {
                if (s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags |= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 0);
                                }
                            }
                        }
                    }
                }
            } else {
                if (s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    m->flags = (s16)m->flags & ~1;
                }
            }
            break;
        case 8:
            for (i = 0; i <= s->last; i++) {
                OccSleep(s->id + i);
            }
            break;
        case 9:
            for (i = 0; i <= s->last; i++) {
                OccsSleep(s->id + i);
            }
            break;
        case 14:
            for (i = 0; i <= s->last; i++) {
                if (MapNearObjGetState(s->id + i) != -1) {
                    MapNearObjSetState(s->id + i, 0);
                } else {
                    FldTreasureSetStateById(s->id + i, 0);
                }
            }
            break;
        case 19:
            SIB(FldSkySetEnable)(0, 0);
            break;
        case 20: {
                ChrObj *t = ChrGetById(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 0);
                }
                break;
            }
        case 21: {
                ChrObj *t = ChrTenantGet(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 0);
                }
                break;
            }
        }
        break;
    case 1:
        switch (OCC_PARAMS(obj, OccSwitch)->target) {
        case 0:
            if (obj->flags & OCC_PART) {
                c = FldObjGet(s->id);
                if (c != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(c, i);
                        if (e != NULL) {
                            e->flags &= ~8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 1);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i <= s->last; i++) {
                    GphModel *c = FldObjGet(s->id + i);
                    if (c != NULL) {
                        c->flags &= ~8;
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i <= s->last; i++) {
                EvtArea *h = EvtAreaGet(s->id + i);
                if (h != NULL) {
                    EvtAreaEnable(h);
                }
            }
            break;
        case 2:
            for (i = 0; i <= s->last; i++) {
                FldSndEmitterEnable(s->id + i, 1);
            }
            break;
        case 3:
            for (i = 0; i <= s->last; i++) {
                FldPointLightSetEnable(s->id + i, 1);
            }
            break;
        case 4:
            for (i = 0; i <= s->last; i++) {
                OccWake(s->id + i);
            }
            break;
        case 5:
            for (i = 0; i <= s->last; i++) {
                OccsWake(s->id + i);
            }
            break;
        case 6:
            if (obj->flags & OCC_PART) {
                m = ChrTenantGet(s->id);
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags &= ~8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 1);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    m = ChrTenantGet(s->id + i);
                    if (m != NULL) {
                        m->flags |= 1;
                    }
                }
            }
            break;
        case 7:
        case 18:
            if (obj->flags & OCC_PART) {
                if (s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags &= ~8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 1);
                                }
                            }
                        }
                    }
                }
            } else {
                if (s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    m->flags |= 1;
                }
            }
            break;
        case 14:
            for (i = 0; i <= s->last; i++) {
                if (MapNearObjGetState(s->id + i) != -1) {
                    MapNearObjSetState(s->id + i, 1);
                } else {
                    FldTreasureSetStateById(s->id + i, 1);
                }
            }
            break;
        case 19:
            SIB(FldSkySetEnable)(0, 1);
            break;
        case 20: {
                ChrObj *t = ChrGetById(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 1);
                }
                break;
            }
        case 21: {
                ChrObj *t = ChrTenantGet(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 1);
                }
                break;
            }
        }
        break;
    case 2:
        if (s->wait > 0) {
            s->wait--;
            obj->timer++;
            break;
        }
        switch (s->ramp) {
        case 0:
            break;
        case 1:
            if (s->cur > 0) {
                s->cur--;
            }
            break;
        case 2:
            if (s->cur < s->interval) {
                s->cur++;
            }
            break;
        }
        s->wait = s->cur;
        switch (s->target) {
        case 0:
            if (obj->flags & OCC_PART) {
                c = FldObjGet(s->id);
                if (c != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(c, i);
                        if (e != NULL) {
                            e->flags ^= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 2);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i <= s->last; i++) {
                    GphModel *c = FldObjGet(s->id + i);
                    if (c != NULL) {
                        c->flags ^= 8;
                    }
                }
            }
            break;
        case 1:
            for (i = 0; i <= s->last; i++) {
                EvtArea *h = EvtAreaGet(s->id + i);
                if (h != NULL) {
                    if (EvtAreaIsEnabled(h) == 0) {
                        EvtAreaEnable(h);
                    } else {
                        EvtAreaDisable(h);
                    }
                }
            }
            break;
        case 2:
            for (i = 0; i <= s->last; i++) {
                FldSndEmitterToggle(s->id + i);
            }
            break;
        case 3:
            for (i = 0; i <= s->last; i++) {
                FldPointLightSetEnable(s->id + i, 2);
            }
            break;
        case 6:
            if (obj->flags & OCC_PART) {
                m = ChrTenantGet(s->id);
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags ^= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 2);
                                }
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i < s->last + 1; i++) {
                    m = ChrTenantGet(s->id + i);
                    if (m != NULL) {
                        m->flags ^= 1;
                    }
                }
            }
            break;
        case 7:
        case 18:
            if (obj->flags & OCC_PART) {
                if ((s16)(u16)s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    for (i = s->part; i <= s->last; i++) {
                        e = ChrModelFindPart(m->model, i);
                        if (e != NULL) {
                            e->flags ^= 8;
                            if (obj->flags & OCC_TREE) {
                                if (e->child != 0) {
                                    OccPartTreeSetFlags(&e[e->child], 8, 2);
                                }
                            }
                        }
                    }
                }
            } else {
                if ((s16)(u16)s->target == 0x12) {
                    m = ChrGetByPartySlot(s->id);
                } else {
                    m = ChrGetById(s->id);
                }
                if (m != NULL) {
                    m->flags ^= 1;
                }
            }
            break;
        case 19:
            if (FldSkyIsEnabled(0) == 0) {
                SIB(FldSkySetEnable)(0, 1);
            } else {
                SIB(FldSkySetEnable)(0, 0);
            }
            break;
        case 20: {
                ChrObj *t = ChrGetById(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 2);
                }
                break;
            }
        case 21: {
                ChrObj *t = ChrTenantGet(s->id);
                if (t != NULL) {
                    SIB(ChrExtraPartsModeSet)(t, 2);
                }
                break;
            }
        }
        break;
    }
}
