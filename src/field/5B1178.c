#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/state.h"
#include "field/n13.h"

void VuMtxStackLoad(Camera *);
extern u8 fldInstancePool[];

void FldSetDrawContext(s32 arg0, s32 arg1, s32 arg2) {
    Camera *o;

    MemHeapSelect(sysDataHeap);
    GphLightListSet(fldState.lightList);
    ChrModelPoolSet(fldInstancePool);
    DbgSetObjList((struct DbgObjList *)&fldModelTable);
    DbgSetCamera(camField);
    o = &camField[arg1];
    CamApply(o);
    VuMtxStackLoad(o);
    GphAnimSetRelocate(0);
}
