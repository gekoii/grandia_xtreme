/* Port-only C for GphModelUpdateGsRegs (0x00434148), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphModelUpdateGsRegs(GphModel *model);

/* For each part drawn last frame (drawn set, GA objects, not hidden):
   rebuild its GS register packet if its first GA object is translucent
   (unk30 > 0) or, with gsFlags bit 0 set, if gsFlags other than bit 0
   changed since the last build (unk52); then point every GA object's gsPkt
   at the part's packet, using the per-frame copies (gaObjsCopy, cleared
   here) when there are any. Parts with gsFlags bit 0 clear and an opaque
   first GA object are left alone. */
void GphModelUpdateGsRegs(GphModel *model) {
    GphPart *part;
    GphGaObject *obj;
    GphGaObject *copy;
    s32 n;
    s32 k;
    s32 pkt;

    if (model->flags & 8) {
        return;
    }
    if (model->cull < 0) {
        return;
    }
    n = model->partCount;
    part = model->parts;
    do {
        obj = part->gaObjs;
        if (part->drawn != 0 && obj != NULL && !(part->flags & 8)) {
            if (0.0f < obj->unk30) {
                goto rebuild;
            }
            if (part->gsFlags & 1) {
                if ((part->gsFlags & ~1) == (part->unk52 & ~1)) {
                    goto apply;
                }
            rebuild:
                GphPartBuildGsRegs(part);
                part->unk52 = part->gsFlags;
                obj = part->gaObjs;
            apply:
                copy = part->gaObjsCopy;
                pkt = part->gsPkt;
                if (copy != NULL) {
                    part->gaObjsCopy = NULL;
                    obj = copy;
                }
                k = part->gaObjCount;
                do {
                    obj->gsPkt = pkt;
                    k--;
                    obj++;
                } while (k > 0);
            }
        }
        part++;
        n--;
    } while (n > 0);
}
