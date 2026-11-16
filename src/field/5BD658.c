#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "vu.h"
#include "field/state.h"
#include "field/event.h"


void VuVecCopy(void *, void *);

void OccMiscForwardStart(OccEntry *obj) {
    OccMisc *s = OCC_PARAMS(obj, OccMisc);
    FVECTOR v;
    s16 buf[8];
    GphModel *m;
    GphModel *mm;
    GphPart *n;
    ChrObj *e;
    s32 r;
    s32 i;

    switch (s->op) {
    case 0:
        ChrPartyGatherToLeader();
        break;
    case 1:
        ChrAllGatherToLeader();
        break;
    case 2:
        ChrNonPartyDeactivate();
        break;
    case 3:
        ChrAllMoveToLeader();
        break;
    case 4:
        camCtl.resetBehind = 1;
        camCtl.unk8 = 0x3C;
        break;
    case 5:
        CamBlendSetTarget();
        break;
    case 6:
        CamBlendStart(s->unk8);
        obj->timer = 1;
        s->wait = 0;
        break;
    case 7:
        fldState.unk2D = s->unk8;
        fldState.unk2C = s->unk2 + 1;
        break;
    case 8:
        ChrAllStateReset();
        break;
    case 9:
        if (ChrIsInParty(7) == 1) {
            ChrPartySwapToSlot(7, 1);
        }
        break;
    case 0x10:
        ChrPartyOrderRestore(0, 0);
        break;
    case 0x11:
        mm = MapModelFindById(s->unk8);
        if (mm != NULL) {
            n = ChrModelFindPart(mm, s->unk2);
            if (n != NULL) {
                v[0] = 1.0f;
                v[1] = 1.0f;
                v[2] = 1.0f;
                v[3] = 1.0f;
                __asm__ volatile(
                    "lqc2 $vf1, 0(%0)\n\t"
                    "lqc2 $vf4, 0x0(%1)\n\t"
                    "lqc2 $vf5, 0x10(%1)\n\t"
                    "lqc2 $vf6, 0x20(%1)\n\t"
                    "lqc2 $vf7, 0x30(%1)\n\t"
                    "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
                    "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
                    "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
                    "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
                    "sqc2 $vf1, 0(%0)"
                    : : "r"(v), "r"(n->mtx) : "memory");
                PtclDustSpawn(v, 1);
            }
        }
        break;
    case 0x12:
        e = ChrGetByNo(1);
        m = MapModelFindById(s->unk8);
        if (m != NULL) {
            VuVecCopy(m->pos, e->model->pos);
            m->rot[1] = e->model->rot[1];
        }
        break;
    case 0x13:
        EvtSystemFlagSet(0x97, (u8)s->unk8);
        break;
    case 0x254:
    case 0x255:
        r = MapSetFileGet();
        if (r != 0 && btlHandoff.dropCount == 0) {
            i = 0;
            btlHandoff.dropCount = VuRandInt(3) + 1;
            for (; i < btlHandoff.dropCount; i++) {
                AmapRollItems(1, s->op - 0x254, r, buf);
                btlHandoff.dropIds[i] = buf[0];
            }
            EffSpawnCode35(ChrGetByNo(1)->model->pos);
            SndSePlay(0x7D7, 0, 0, 0x1E, 0);
        }
        break;
    }
}
