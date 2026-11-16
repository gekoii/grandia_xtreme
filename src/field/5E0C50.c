#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n17.h"


void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void PtclFldSystemCreate(void) {
    s32 i;
    s32 j;
    s32 na;
    s32 nb;
    s32 size;
    void *pool;
    void *t;
    PtclPtbHeader *t2;
    PtclCtrl *e;
    PtclGenRequest *l;
    GphAnimSet *m;
    PtclEffector *src;
    PtclEffector *dst;

    na = 0;
    nb = 0;
    for (i = 0; i < 16; i++) {
        if (ptclFldWork->res[i] != NULL && ptclFldWork->res[i]->ptb != NULL) {
            t = ptclFldWork->res[i]->ptb;
            na += ((PtclPtbHeader *)t)->effectorCount;
            nb += ((PtclPtbHeader *)t)->ctrlCount;
        }
    }
    size = PtclSystemCalcSize(0x40, na, nb);
    pool = FldMemAlloc(size);
    MemSet(pool, 0, size);
    PtclSystemInit(pool, 0x40, na, nb);
    ptclFldWork->system = pool;
    PtclSystemSelect((s32)pool);
    for (i = 0; i < 16; i++) {
        if (ptclFldWork->res[i] == NULL) {
            continue;
        }
        t2 = ptclFldWork->res[i]->ptb;
        if (t2 == NULL) {
            continue;
        }
        for (j = 0; j < t2->ctrlCount; j++) {
            e = (PtclCtrl *)((u8 *)t2 + t2->ctrlOffset) + j;
            l = FldMemAlloc(sizeof(PtclGenRequest));
            ptclFldWork->links[i][j] = l;
            MemSet(l, 0, sizeof(PtclGenRequest));
            l->ctrl = e;
            t = PtclPtbGetString(t2, e->resIndex);
            switch (*(s8 *)t) {
            case 1:
                l->texId = ptclFldWork->texIds[i][e->resIndex];
                l->animSet = 0;
                l->model = 0;
                break;
            case 3:
                m = ptclFldWork->res[i]->animSets[e->resIndex];
                if (m != NULL) {
                    l->texId = ((GphTex *)m->tex)->id;
                    l->animSet = (s32)m;
                } else {
                    l->texId = 0;
                    l->animSet = 0;
                }
                l->model = 0;
                break;
            default:
                l->model = (void *)FilePacGetModel(ptclFldWork->modelPacs[i], e->resIndex);
                l->texId = 0;
                break;
            }
            l->partPrio = 10;
            ptclFldWork->slots[i][j] = PtclStarterSlotAlloc(e);
        }
        src = (PtclEffector *)((u8 *)t2 + t2->effectorOffset);
        for (j = t2->effectorCount; j > 0; j--) {
            dst = PtclEffectorRegister(src->id);
            if (dst != NULL) {
                *dst = *src;
            }
            src++;
        }
    }
}
