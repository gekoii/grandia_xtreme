#include "common.h"
#include "protos/viewer.h"
#include "sys.h"
#include "viewer/n35.h"

void VuMtxStackLoad(void *);

void VwrViewSetup(s32 arg0) {
    MemHeapSelect(sysDataHeap);
    GphDrawListSet(vwrWork.mainList);
    GphLightListSet(vwrWork.lightList);
    DbgSetObjList(&vwrModelList);
    DbgSetCamera(&vwrCamera);
    vwrCamera.viewNo = arg0;
    CamApply(&vwrCamera);
    VuMtxStackLoad(&vwrCamera);
}
