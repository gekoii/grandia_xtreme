#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "gph.h"
#include "party.h"
#include "battle/camera.h"
#include "battle/misc.h"
#include "battle/n31.h"

s32 MemMove128(void *, s32, s32);
void FlushCache(s32);
void func_00748000(MgcWork *);

void MgcUpdate(void) {
    MgcWork *m = &mgcWork;
    s32 r;
    s32 i;
    GphModel *o;
    BtlUnit *c;
    s32 idx;

    if (mgcWork.state == 0) {
        return;
    }
    MemHeapSelect(sysState.battleMagicHeap);
    GphTexTableSelect(m->textures);
    PtclSystemSelect(m->ptclSystem);
    if (mgcWork.state == 1) {
        r = BtlFileLoadAsync(NULL, 0);
        if (r != 0) {
            if (m->update == NULL) {
                if (!(btlDebugCfg.flags & 0x40)) {
                    MemMove128(func_00748000, BtlPakGetEntry(r, 9), BtlPakGetEntrySize(r, 9) / 16);
                }
                FlushCache(0);
                FlushCache(2);
                func_00748000(m);
            }
            m->state = 2;
        }
    }
    if (m->state == 4) {
        GphShatterStart(9);
        if (m->update != NULL) {
            m->update(m);
            MgcPtclGroupsUpdate();
            PtclUpdate();
        }
        if (m->partStart != NULL) {
            ((s32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(m->caster);
            m->partStart(m);
        }
        m->state = 5;
    } else if (m->state == 5) {
        if (m->update != NULL) {
            m->model = 0;
            m->update = NULL;
            m->draw = NULL;
            MgcCleanup();
            mgcEndFrame = btlFrameCount;
        }
        if (MgcPtclGroupsRelease() != 0 && FxAfterimageStop() != 0 && FxCloneIsFree() != 0 && mgcEndFrame != btlFrameCount) {
            c = m->caster;
            if (c->id < 4 && c->action == 3) {
                idx = m->part - 1;
                m->unk13 = ptyPartyData->eggs[c->eggNo[idx]].spellUpgrades[c->eggMagic[idx]];
            } else {
                m->unk13 = 0;
            }
            MgcSelectHitData(0);
            for (i = 0; i < 256; i++) {
                if (m->modelData[i] != NULL) {
                    o = *m->models[i];
                    ChrModelRestorePartAttr(o, m->modelData[i]);
                    MgcModelResetPartDisp(o, m->modelData[i]);
                    o->motion[0].frame = 0;
                    o->flags |= 8;
                    ChrModelUvScrollReset(o);
                }
            }
            m->caster->scopeRadius = BtlSpellGetLevelData(m->caster, m->part - 1, 0x10)->scope;
            EnvDimForSkill(m->caster, ptyMoveTable[m->id].unkF + 0x10);
            m->state = 2;
            m->phase = 0x100;
            m->draw = MgcProcNop;
            if (m->drawList != 0) {
                GphDrawListSet(m->drawList);
                GphDrawListClear(NULL);
            }
            m->nextPart(m);
        }
    }
    if (m->state == 2 && m->update != NULL) {
        m->update(m);
        MgcPtclGroupsUpdate();
        PtclUpdate();
        m->count++;
    }
    if (m->state == 3) {
        if (m->update != NULL) {
            m->model = 0;
            m->update = NULL;
            m->draw = NULL;
            if ((btlCamParam.flags & 2) && btlCamParam.mode != 1) {
                BtlCamSetMode(1, 0x40, 0, 0);
            }
            GphScreenFxSet(&btlScreenFx, 0, 1, 0, 0.0f);
            MgcCleanup();
            SndSeStopGroup(2);
            mgcEndFrame = btlFrameCount;
        }
        if (MgcPtclGroupsRelease() != 0 && FxAfterimageStop() != 0 && FxCloneIsFree() != 0 && mgcEndFrame != btlFrameCount) {
            m->state = 0;
            mgcAbortStep = 0;
            MemHeapReset(sysState.battleMagicHeap);
        }
    }
    MemHeapSelect(sysState.battleHeap);
    GphTexTableSelect(btlTexTable);
}
