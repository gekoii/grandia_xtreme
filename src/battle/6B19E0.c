/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006E13F0-0x006E15F8 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "ptcl.h"
#include "party.h"
#include "chr.h"
#include "battle/camera.h"
#include "battle/misc.h"
#include "battle/n31.h"
#include "battle/n32.h"

static const char save[] = "save";



void MgcDraw(void) {
    MgcWork *m = &mgcWork;

    if (m->draw != NULL || m->model != NULL) {
        if (SysIsDebugMode() != 0) {
            DbgPrintf(0x2A, 4, "P:%3X C:%d", m->phase, m->count);
        }
        GphTexTableSelect(m->textures);
        PtclSystemSelect(m->ptclSystem);
        if (m->drawList != 0) {
            GphDrawListSet(m->drawList);
            GphDrawListDrawAll();
        }
        if (m->draw != NULL) {
            m->draw(m);
        }
        PtclDrawSprites(0);
        GphTexTableSelect(btlTexTable);
    }
}

void MgcProcNop(MgcWork *m) {

}

void MgcBuildDrawList(void) {
    MgcWork *m = &mgcWork;

    if (m->model != NULL && m->drawList != 0) {
        EnvLightSelectDim(0);
        GphDrawListSet(m->drawList);
        GphDrawListClear(NULL);
        m->model(m);
        PtclSystemSelect(m->ptclSystem);
        PtclDrawModels(0);
        GphDrawListSort();
        GphDrawListSet(0);
    }
}

void MgcEndFrame(void) {
    MgcWork *m = &mgcWork;
    if (m->model != NULL && m->drawList != 0) {
        PtclSystemSelect(m->ptclSystem);
        PtclModelsUpdateGsRegs();
    }
}


void MgcPtclGroupLoad(MgcPtclGroup *st, PtclPtbHeader *h) {
    s32 i;
    s32 n;
    s32 c;
    MgcWork *m;
    PtclStarterEntry *e;
    PtclCtrl *r;
    PtclEffector *p;
    PtclEffector *d;

    m = &mgcWork;
    i = 0;
    st->ptb = h;
    c = h->starterCount;
    st->frame = -1;
    if (c > 16) {
        c = 16;
    }
    st->count = c;
    e = (PtclStarterEntry *)((u8 *)h + h->starterOffset);
    for (; i < st->count; i++, e++) {
        if (e->ctrl >= 0) {
            r = (PtclCtrl *)((u8 *)st->ptb + st->ptb->ctrlOffset) + e->ctrl;
            if (*PtclPtbGetString(h, r->resIndex) == 2) {
                mgcPtclModelCount += m->resources[r->resIndex]->gaObjCount * r->groupCount;
            }
        }
    }
    n = h->effectorCount;
    p = (PtclEffector *)((u8 *)h + h->effectorOffset);
    for (; n > 0; n--, p++) {
        d = PtclEffectorRegister(p->id);
        if (d != NULL) {
            *d = *p;
        }
    }
}

void MgcPtclGroupAdd(MgcPtclGroup *g) {
    MgcWork *m = &mgcWork;
    s32 i;
    s32 j;

    if (g->frame < 0) {
        for (j = 0; j < 256; j++) {
            if (m->ptclGroups[j] == NULL) {
                g->frame = 0;
                for (i = 15; i >= 0; i--) {
                    g->slots[i] = NULL;
                }
                g->pos[0] = g->pos[1] = g->pos[2] = 0.0f;
                g->rot[2] = 0;
                g->rot[1] = 0;
                g->rot[0] = 0;
                m->ptclGroups[j] = g;
                return;
            }
        }
    }
}

void MgcPtclGroupStop(MgcPtclGroup *g) {
    s32 i;
    PtclStarterSlot *o;

    if (g->frame >= 0) {
        for (i = 0; i < 16; i++) {
            o = g->slots[i];
            if (o != NULL && PtclGenIsAlive(o->gen) != 0) {
                o->gen->flags |= 0x400;
            }
        }
        g->frame = 999999;
    }
}

void MgcPtclGroupRelease(MgcPtclGroup *g) {
    s32 i;
    PtclStarterSlot *o;

    if (g->frame >= 0) {
        for (i = 0; i < 16; i++) {
            o = g->slots[i];
            if (o != NULL && PtclGenRelease(o->gen) != 0) {
                MemFree(o->gen);
                PtclStarterSlotFree((s32 *)o);
                g->slots[i] = NULL;
            }
        }
        g->frame = 999999;
    }
}

s32 MgcPtclGroupIsDone(MgcPtclGroup *g) {
    s32 ok = 1;
    s32 i;
    PtclStarterSlot *o;

    if (g->frame < 0) {
        return 1;
    }
    for (i = 0; i < 16; i++) {
        o = g->slots[i];
        if (o != NULL) {
            if (PtclGenIsAlive(o->gen) != 0) {
                ok = 0;
            } else if (PtclGenRelease(o->gen) != 0) {
                MemFree(o->gen);
                PtclStarterSlotFree((s32 *)o);
                g->slots[i] = NULL;
            }
        }
    }
    return ok;
}

s32 MgcPtclGroupsAreDone(void) {
    s32 ok = 1;
    s32 i;

    for (i = 0; i < 256; i++) {
        if (mgcWork.ptclGroups[i] != NULL) {
            if (MgcPtclGroupIsDone(mgcWork.ptclGroups[i]) == 0) {
                ok = 0;
            }
        }
    }
    return ok;
}

s32 MgcPtclGroupsRelease(void) {
    s8 total = 0;
    s32 save;
    s32 i;
    s32 j;
    s8 cnt;
    PtclStarterSlot *o;
    MgcPtclGroup *g;

    MgcWork *m = &mgcWork;

    save = PtclSystemGetCur();
    PtclSystemSelect(m->ptclSystem);
    for (i = 0; i < 256; i++) {
        if (m->ptclGroups[i] != NULL) {
            cnt = 0;
            for (j = 0; j < 16; j++) {
                o = m->ptclGroups[i]->slots[j];
                if (o != NULL) {
                    if (PtclGenRelease(o->gen) != 0) {
                        MemFree(o->gen);
                        PtclStarterSlotFree((s32 *)o);
                        m->ptclGroups[i]->slots[j] = NULL;
                    } else {
                        cnt++;
                    }
                }
            }
            if (cnt == 0) {
                g = m->ptclGroups[i];
                m->ptclGroups[i] = NULL;
                g->frame = -1;
            } else {
                total++;
            }
        }
    }
    PtclSystemSelect(save);
    return total == 0;
}

void MgcCleanup(void) {
    s32 i;
    s32 *p = mgcLightIds;

    for (i = 8; i > 0; i--) {
        if (*p >= 0) {
            EnvLightFree(*p);
            *p = -1;
        }
        p++;
    }
    EnvShadowSetLightBlend(0.0f);
    EnvStageShadeFade(1, 0.0f);
    if (!(btlCamParam.flags & 4)) {
        BtlCamQuakeStop(0);
    }
    FxAfterimageStop();
    FxCloneFadeOut(0);
}

/* arg0: a GphModel ** filled by MgcModelsCreate; arg1: its ChrModelData. */
void MgcModelRegister(s32 *arg0, s32 arg1) {
    s32 i;
    MgcWork *m = &mgcWork;
    s32 **q = (s32 **)m->models;
    s32 *p = (s32 *)m->modelData;
    for (i = 0; i < 256; i++, q++, p++) {
        if (*p == 0) {
            *p = arg1;
            *q = arg0;
            return;
        }
    }
    *arg0 = 0;
}


void MgcModelsCreate(void *arg0) {
    MgcWork *m = &mgcWork;
    s32 i;
    s32 tot = 0;
    s32 sum = 0;
    GphModel *o;

    for (i = 0; i < 256; i++) {
        if (m->modelData[i] != NULL) {
            tot += ChrModelCalcInstanceSize(m->modelData[i]);
            sum += m->modelData[i]->gaObjCount;
        }
    }
    ChrModelPoolInit(arg0, MemAlloc128(tot), tot);
    m->drawList = MemAlloc32(GphDrawListSize(sum + mgcPtclModelCount));
    GphDrawListInit(sum + mgcPtclModelCount, m->drawList);
    GphDrawListSet(m->drawList);
    GphDrawListClear(NULL);
    ChrModelPoolSet(arg0);
    for (i = 0; i < 256; i++) {
        if (m->modelData[i] != NULL) {
            o = BtlModelCreate((s32)m->modelData[i], 10);
            *m->models[i] = o;
            MgcModelResetPartDisp(o, m->modelData[i]);
            o->motion[0].frame = 0;
            o->flags |= 8;
            ChrModelUvScrollReset(o);
        }
    }
    ChrModelPoolSet(NULL);
}

s32 MgcModelsMotionDone(void) {
    s32 i;
    GphModel **e;
    GphModel *o;
    GphMotion *a;

    for (i = 0; i < 256; i++) {
        e = mgcWork.models[i];
        if (e == NULL) {
            continue;
        }
        o = *e;
        if (o->flags & 8) {
            continue;
        }
        a = o->motion[0].motion;
        if (a == NULL) {
            continue;
        }
        if (a->playFlags & 1) {
            continue;
        }
        if (o->motion[0].frame < (f32)a->frameCount - 1.1f) {
            return 0;
        }
    }
    return 1;
}


/* s: a texture (flag 0) or a 2D anim set and its texture (flag 1) */
void MgcTexRegister(GphAnimSet *s, s8 flag, s32 unused) {
    if (flag == 0) {
        UtilTexRegister(s); return;
    }
    GphAnimSetRelocate(s);
    UtilTexRegister(s->tex);
}


void MgcTexLoadAll(PtclPtbHeader *h, s32 arg1, s8 arg2) {
    s32 i;
    s8 *s;
    void *v;
    s8 flag;
    s8 c;
    MgcWork *m;

    m = &mgcWork;
    s = (s8 *)h + h->stringOffset;
    for (i = 0; i < h->stringCount && i < 8; i++) {
        v = BtlPakGetEntry(arg1, arg2 + i);
        m->resources[i] = v;
        c = *s;
        switch (c) {
        case 1:
            flag = 0;
            goto call; /* goto into case 3: two calls (or one with c == 3) change the branch layout */
        case 3:
            flag = 1;
        call:
            MgcTexRegister(v, flag, c);
            break;
        }
        while (*s != 0) {
            s++;
        }
        s++;
    }
}


s32 MgcModelPackLoad(s32 arg0) {
    s32 r = FilePacModelRelocate(arg0);
    FilePacModelBindTextures(r);
    return r;
}

void *MgcRelocateForUnit(u8 *arg0, BtlUnit *arg1) {
    FileChunkRelocate((s32 *)arg0);
    arg0 += 0x10;
    BtlChrTexRemap((ChrModelData *)arg0, gphTexTableBuf, arg1->def->modelPack);
    return arg0;
}

s32 MgcStartNextHit(void) {
    BtlUnit *c;

    if (mgcWork.nextPart == 0) {
        return 0;
    }
    mgcWork.state = 4;
    mgcWork.part++;
    c = mgcWork.caster;
    if (c->body >= 0) {
        BtlUnit *u = &btlUnits[c->body];
        u->phase = 0x100;
    } else {
        c->phase = 0x100;
    }
    return 1;
}

#define ROUNDUP(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))


void MgcApplyHitDamage(void) {
    BtlUnit *c = mgcWork.caster;
    s32 i;
    s32 end;
    s16 v;
    BtlUnit *u;
    f32 sc;

    if (c->id < 4) {
        i = 4;
        end = 16;
        v = BtlSpellGetLevelData(c, mgcWork.part - 1, 1)->mpCost;
    } else {
        i = 0;
        end = 4;
        v = c->def->data->actionSets[c->ai].actions[c->action]->level->mpCost;
    }
    u = &btlUnits[i];
    for (; i < end; i++, u++) {
        if ((btlTargetMask >> i) & 1) {
            sc = u->params->takeDmgMp;
            if (sc > 0.0f) {
                BtlHitQueueAddDamage(u, ROUNDUP(v * sc), 0.0f, u->rot[1], 0x140, 0);
            }
        }
    }
}

#undef ROUNDUP

void MgcSelectHitData(s32 arg0) {
    MgcWork *m = &mgcWork;
    BtlUnit *c = m->caster;
    PtyItemUse *s;
    PtyMoveLevelData *e;
    PtyMoveLevelData *e3;
    PtyMoveLevelData *e4;
    s32 n;

    if (c == NULL) {
        return;
    }
    if (c->id < 4) {
        if (c->action == 2) {
            s = ptyItemTable[c->skillId].use;
            m->hitData = s->effects;
            n = s->effectCount;
        } else if (c->action == 3) {
            e3 = BtlSpellGetLevelData(c, m->part - 1, 0x46);
            m->hitData = e3->effects;
            n = e3->effectCount;
        } else {
            e = &ptyMoveTable[c->skillId].levels[c->skillLevel - 1];
            m->hitData = e->effects;
            n = e->effectCount;
        }
    } else if (c->skillId == 0x5F) {
        m->hitData = NULL;
        return;
    } else {
        e4 = c->def->data->actionSets[c->ai].actions[c->action]->level;
        m->hitData = e4->effects;
        n = e4->effectCount;
    }
    if (arg0 >= n) {
        arg0 = n - 1;
    }
    m->hitData += arg0;
}

void MgcSetActorInfo(BtlUnit *c, PtyMoveEffect *arg1) {
    MgcWork *m = &mgcWork;
    m->actorId = c->id;
    m->actorAction = c->action;
    m->actorSkillId = c->skillId;
    m->actorBody = c->body;
    *(s32 *)&m->hitData = (s32)arg1; /* s32 store: keeps the old alias order */
    m->unkC98 = 0;
    m->actorParams = *(s32 *)&c->params; /* s32 load: a pointer cast swaps the two zero stores */
    m->unkC9C = 0;
}

PtyMoveEffect *MgcGetHitData(void) {
    return mgcWork.hitData;
}

void func_00748000(void);

static inline s32 sub_idx(s32 a, s32 b) {
    return a - (b - 1) * 6;
}

void MgcLoadFile(s8 flags, char *name) {
    char buf[256];
    s32 id = mgcWork.id;
    BtlUnit *c = mgcWork.caster;
    s32 lo = id < 91;
    s32 k;

    if (lo) {
        if ((flags & 1)) {
            sprintf(buf, "BTL_DATA/M%3.1d.DAT", id);
        } else {
            sprintf(buf, "BATTLE/M%3.1d_0.GBX", id);
        }
        BtlStrSpaceToZero(buf);
    } else {
        if (c == NULL && id >= 201 && id % 10 == 2) {
                if ((flags & 1)) {
                sprintf(buf, "BTL_DATA/E%.3XS%.1d.DAT", 0xD1, 2);
            } else {
                sprintf(buf, "BATTLE/E%.3XS%.1d_0.GBX", 0xD1, 2);
            }
        } else if (!(flags & 1) && *name != '?') {
            sprintf(buf, "BATTLE/%s_0.GBX", name);
        } else {
            k = c->def->id;
            if (c->id < 4) {
                k -= 0x1000;
                if (id >= 241) {
                    sprintf(buf, (flags & 1) ? "BTL_DATA/PC_CMB.DAT" : "BATTLE/PC_CMB_0.GBX");
                } else if (id >= 221) {
                    sprintf(buf, (flags & 1) ? "BTL_DATA/U%.3d.DAT" : "BATTLE/U%.3d_0.GBX", id);
                } else {
                    sprintf(buf, (flags & 1) ? "BTL_DATA/P%.3XS%.1d.DAT" : "BATTLE/P%.3XS%.1d_0.GBX", k, sub_idx(id - 100, k));
                }
            } else {
                if ((k & 0xFF) > 80 && (k & 0xFF) != 0xA4) {
                    k &= 0xFF;
                }
                switch (id) {
                case 91:
                    if ((flags & 1)) {
                        sprintf(buf, "BTL_DATA/E%.3XCL.DAT", k);
                    } else if (k == 0x51 || k == 0xA1 || k == 0xD1) {
                        sprintf(buf, "BATTLE/E%.3XCL_0.GBX", k);
                    } else {
                        sprintf(buf, "BATTLE/E_CALL_0.GBX");
                    }
                    break;
                case 92:
                    sprintf(buf, (flags & 1) ? "BTL_DATA/E_AWAY.DAT" : "BATTLE/E_AWAY_0.GBX");
                    break;
                case 93:
                case 94:
                    sprintf(buf, (flags & 1) ? "BTL_DATA/E_CMB.DAT" : "BATTLE/E_CMB_0.GBX");
                    break;
                case 95:
                    if ((flags & 1)) {
                        sprintf(buf, "BTL_DATA/E%.3XD.DAT", k);
                    } else {
                        sprintf(buf, "BATTLE/B_DEAD_0.GBX");
                    }
                    break;
                default:
                    if (id < 148) {
                        sprintf(buf, (flags & 1) ? "BTL_DATA/P%.3XS%.1d.DAT" : "BATTLE/P%.3XS%.1d_0.GBX", (id - 101) / 6 + 1, (id - 101) % 6 + 1);
                    } else if ((flags & 1)) {
                        sprintf(buf, "BTL_DATA/E%.3XS%.1d.DAT", k, id % 10);
                    } else {
                        sprintf(buf, "BATTLE/E%.3XS%.1d_0.GBX", k & 0xFF, id % 10);
                    }
                    break;
                }
            }
        }
    }
    if ((flags & 0x10) && FileGetSizeAligned(buf) <= 0) {
        BtlFileLoadSetPending(1);
        if (lo) {
            mgcWork.update = MgcFallbackMagicStart;
            mgcWork.partStart = MgcFallbackCamReset;
            mgcWork.nextPart = MgcFallbackNextPart;
        } else {
            mgcWork.update = MgcFallbackSkillStart;
        }
    } else if ((flags & 1)) {
        BtlFileLoadAsync(buf, NULL);
    } else {
        BtlFileLoadAsync(buf, func_00748000);
    }
}

