#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/n17.h"
#include "main/n07.h"

void PtclFldEffectLoad(s32 arg0, PtclFldRes *arg1) {
    s32 i;
    GphModel *o;
    GphAnimSet *m;

    ptclFldWork->res[arg0] = arg1;
    if (arg1->modelPac != 0) {
        arg1->modelPac += (s32)arg1;
    }
    if (ptclFldWork->res[arg0]->motion != 0) {
        ptclFldWork->res[arg0]->motion += (s32)arg1;
    }
    if (ptclFldWork->res[arg0]->unk10 != 0) {
        ptclFldWork->res[arg0]->unk10 += (s32)arg1;
    }
    if (ptclFldWork->res[arg0]->ptb != 0) {
        ptclFldWork->res[arg0]->ptb = (PtclPtbHeader *)((s32)ptclFldWork->res[arg0]->ptb + (s32)arg1);
    }
    for (i = 0; i < 5; i++) {
        if (ptclFldWork->res[arg0]->textures[i] != 0) {
            ptclFldWork->res[arg0]->textures[i] += (s32)arg1;
            GphTexDataRelocate(ptclFldWork->res[arg0]->textures[i]);
            ptclFldWork->texIds[arg0][i] = GphTexRegister(ptclFldWork->res[arg0]->textures[i]);
        }
        if (ptclFldWork->res[arg0]->animSets[i] != 0) {
            ptclFldWork->res[arg0]->animSets[i] = (GphAnimSet *)((s32)ptclFldWork->res[arg0]->animSets[i] + (s32)arg1);
            GphAnimSetRelocate(ptclFldWork->res[arg0]->animSets[i]);
            m = ptclFldWork->res[arg0]->animSets[i];
            GphTexDataRelocate((s32)m->tex);
            ((GphTex *)m->tex)->id = GphTexRegister((s32)m->tex);
        }
    }
    if (ptclFldWork->res[arg0]->modelPac != 0) {
        ptclFldWork->modelPacs[arg0] = FilePacModelRelocate(ptclFldWork->res[arg0]->modelPac);
    }
    if (ptclFldWork->res[arg0]->motion != 0) {
        FileChunkRelocate((s32 *)ptclFldWork->res[arg0]->motion);
        ptclFldWork->res[arg0]->motion += 0x10;
    }
    if (ptclFldWork->res[arg0]->modelPac != 0) {
        FilePacModelBindTextures(ptclFldWork->modelPacs[arg0]);
        for (i = 0; i < ((FilePacModelPack *)ptclFldWork->modelPacs[arg0])->modelCount; i++) {
            o = ChrModelCreate(FilePacGetModel(ptclFldWork->modelPacs[arg0], i));
            ptclFldWork->models[arg0][i] = o;
            o->motion[0].motion = (GphMotion *)ptclFldWork->res[arg0]->motion;
            o->motion[0].frame = 0.0f;
            o->motionMode = 0;
            o->blend = 0;
            ChrModelClearDeformBufs(o);
            PtclFldModelClearPartFlag2(ptclFldWork->models[arg0][i]);
        }
    }
}
