#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/state.h"
#include "field/n17.h"


void ChrPartyLoadMember(u32 arg0, s32 arg1) {
    ChrPackBinding *l = &fldPartyPackTable[arg0];
    ChrObj *o;
    void *q;
    s32 i;

    if (arg0 < 9) {
        i = 0;
        ChrPackBind(l, sysPartyPac.packs[arg0]);
        ChrPartyObjCreate(l->modelPac, arg0, arg1);
        o = ChrGetByNo(arg0 + 1);
        q = &o->faceAnim;
        ChrFaceAnimInit(q, l, o->model);
        ChrFaceAnimSet(q, 0, 1);
        do {
            if (l->pack->motions[i] != 0) {
                ChrMotTableSet(o, l->pack->motions[i], i);
            }
            i++;
        } while (i < 32);
    }
}
