#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern u8 GphMeshClipTest(AmapCellInst *);
extern u8 gphDefaultGsRegsPacket[];

void AmapDraw(void) {
    u32 i;
    s32 e;

    if (amapState->loaded != 0 && FldMenuIsActive() == 0) {
        GphTexBind(amapState->texId);
        for (i = 0; i < amapState->cellInstCount; i++) {
            e = sizeof(AmapCellInst);
            e = i * e;
            ((AmapCellInst *)(e + (u32)amapCellInsts))->clip = GphMeshClipTest((AmapCellInst *)((u32)amapCellInsts + e));
            e += (u32)amapCellInsts;
            if (((AmapCellInst *)e)->clip != 0xFF) {
                GphVu1Render((AmapCellInst *)e, gphDefaultGsRegsPacket);
            }
        }
    }
}
