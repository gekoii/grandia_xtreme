#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "grxsys.h"


void PtclBurstLoad(void) {
    ptclDebrisModel = FilePacGetModel(btlSysModelPack, 1);
    ptclAnimSetTex3D = (GphAnimSet *)sysGrxsysPac->animSets[5];
    GphTexRegisterAt(ptclAnimSetTex3D->tex, 0x3D);
    ptclAnimSetTex3B = (GphAnimSet *)sysGrxsysPac->animSets[3];
    GphTexRegisterAt(ptclAnimSetTex3B->tex, 0x3B);
    ptclAnimSetTex3A = (GphAnimSet *)sysGrxsysPac->animSets[2];
    GphTexRegisterAt(ptclAnimSetTex3A->tex, 0x3A);
    ptclAnimSetTex3C = (GphAnimSet *)sysGrxsysPac->animSets[4];
    GphTexRegisterAt(ptclAnimSetTex3C->tex, 0x3C);
    ptclAnimSetTex38 = (GphAnimSet *)sysGrxsysPac->animSets[0];
    GphTexRegisterAt(ptclAnimSetTex38->tex, 0x38);
    ptclAnimSetTex39 = (GphAnimSet *)sysGrxsysPac->animSets[1];
    GphTexRegisterAt(ptclAnimSetTex39->tex, 0x39);
}
