#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n12.h"

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void PtclFldUpdate(void) {
    s32 i;
    s32 j;
    s32 k;
    PtclPtbHeader *t;
    PtclStarterEntry *e;
    PtclGen *p;

    if (ptclFldWork != NULL && ptclFldWork->system != NULL) {
        GphDrawListSet(fldPtclDrawList);
        ptclFldWork->spinA = (f32)ptclFldWork->spinA + 109.226677f;
        ptclFldWork->spinB = (f32)ptclFldWork->spinB - 109.226677f;
        for (i = 0; i < 16; i++) {
            if (ptclFldWork->active[i] && ptclFldWork->stop[i] != 1) {
                if (ptclFldWork->res[i]->ptb != NULL) {
                    t = ptclFldWork->res[i]->ptb;
                    for (j = 0; j < t->starterCount; j++) {
                        e = (PtclStarterEntry *)((u8 *)t + t->starterOffset) + j;
                        if (e->ctrl >= 0) {
                            k = e->ctrl;
                            if (e->time == ptclFldWork->frame[i][k]) {
                                /* one argument through a cast: a1 is left as it is (see PtclGenCalcSize) */
                                p = FldMemAlloc(((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(ptclFldWork->links[i][k]));
                                ptclFldWork->slots[i][k]->gen = p;
                                MemSet(p, 0, 4);
                                PtclGenCreate(ptclFldWork->slots[i][k]->gen, ptclFldWork->links[i][k]);
                                ptclFldWork->slots[i][k]->gen->pos[0] = e->pos[0] + ptclFldWork->offset[i][0];
                                ptclFldWork->slots[i][k]->gen->pos[1] = e->pos[1] + ptclFldWork->offset[i][1];
                                ptclFldWork->slots[i][k]->gen->pos[2] = e->pos[2] + ptclFldWork->offset[i][2];
                                ptclFldWork->slots[i][k]->gen->rot[0] = e->rot[0] + ptclFldWork->rot[i][0];
                                ptclFldWork->slots[i][k]->gen->rot[1] = e->rot[1] + ptclFldWork->rot[i][1];
                                ptclFldWork->slots[i][k]->gen->rot[2] = e->rot[2] + ptclFldWork->rot[i][2];
                            } else if (e->time < ptclFldWork->frame[i][k]) {
                                if (ptclFldWork->slots[i][k]->gen == NULL) {
                                    continue;
                                }
                                PtclGenIsAlive(ptclFldWork->slots[i][k]->gen);
                            }
                            ptclFldWork->frame[i][j]++;
                        }
                    }
                }
            }
        }
        for (i = 0; i < 16; i++) {
            if (ptclFldWork->active[i] && ptclFldWork->stop[i]) {
                if (PtclFldEffectRelease(i)) {
                    ptclFldWork->active[i] = 0;
                }
            }
        }
        PtclSystemSelect((s32)ptclFldWork->system);
        PtclUpdate();
        for (i = 0; i < 16; i++) {
            if (ptclFldWork->active[i]) {
                for (j = 0; j < 8; j++) {
                    if (ptclFldWork->models[i][j]) {
                        PtclFldModelSpinParts(ptclFldWork->models[i][j]);
                        ChrModelUpdate(ptclFldWork->models[i][j], 1.0f);
                    }
                }
            }
        }
    }
}
