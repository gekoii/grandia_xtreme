#include "common.h"
#include "battle/camera.h"
#include "sys.h"
#include "protos/battle.h"

void VuMtxStackLoad(Camera *);

void BtlSetDrawContext(s32 arg0) {
    MemHeapSelect(sysBattleHeap);
    btlCamera->viewNo = arg0;
    VuMtxStackLoad(btlCamera);
}
