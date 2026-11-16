#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void GphModelUpdateGsRegs(GphModel *);

void VwrEndFrame(void) {
    u32 i;

    for (i = 0; i < vwrModelList.count; i++) {
        if (vwrModelList.items[i] != NULL) {
            GphModelUpdateGsRegs(vwrModelList.items[i]);
        }
    }
    /* do-while(0): keeps the final call a jal (no tail call) */
    do {
        PtclModelsUpdateGsRegs();
    } while (0);
}
