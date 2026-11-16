/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/unit.h"
#include "party.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphModelCalcCull(void *, f32);

void BtlEnemyPartDieModelCb(BtlUnit *arg0) {
    BtlUnit *u;
    GphPart *n;
    GphPart *m;
    s32 i;
    s32 j;
    s32 id;
    s32 k;
    f32 t;
    f32 x;
    f32 y;
    f32 z;

    u = &btlUnits[arg0->body];
    switch (arg0->unk2D) {
    case 1:
        for (j = 0; j < 2; j++) {
            id = arg0->def->dieParts[j];
            if (id > 0) {
                n = ChrModelFindPart(u->model, id);
                if (n != NULL) {
                    BtlChrPartsSetGaObjFlag2(n, 1);
                    BtlChrPartsSetAttr(n, 0xB, 1);
                    BtlChrPartsSetAttr(n, 2, 1);
                    BtlChrPartsSetAttr(n, 0xD, 0);
                    BtlChrPartsSetAttr(n, 3, 1);
                    BtlChrPartsSetUvScroll(n, 8, 8);
                }
            }
            id = arg0->def->dieParts2[j];
            if (id > 0) {
                ChrPartDispSet(u, id, 1);
            }
        }
        GphModelCalcCull(u->model, 0.0f);
        arg0->unk2D = 0;
        break;
    case 2:
        for (i = 0; i < 2; i++) {
            id = arg0->def->dieParts[i];
            if (id > 0) {
                ChrPartDispSet(u, id, 0);
            }
        }
        BtlUnitSetPartHook(arg0, NULL);
        GphModelCalcCull(u->model, 0.0f);
        arg0->unk2D = 0;
        return;
    }
    if (!(u->alpha <= 0.0f) && !(u->params->status & 1)) {
        for (i = 0; i < 2; i++) {
            id = arg0->def->dieParts[i];
            if (id > 0) {
                m = ChrModelFindPart(u->model, id);
                if (m != NULL) {
                    t = u->alpha * arg0->alpha;
                    x = (arg0->ambient[0] + 128.0f) * t - 128.0f;
                    y = (arg0->ambient[1] + 128.0f) * t - 128.0f;
                    z = (arg0->ambient[2] + 128.0f) * t - 128.0f;
                    arg0->ambient[3] = (t - 1.0f) * 128.0f;
                    for (k = m->groupPartCount; k > 0; k--, m++) {
                        if ((m->gsFlags >> 11) & 3) {
                            m->color[0] = x;
                            m->color[1] = y;
                            m->color[2] = z;
                        } else {
                            copyq(&m->color[0], &arg0->ambient[0]);
                        }
                    }
                }
            }
        }
    }
}
