/* rodata: 0x00675778-0x006757F8 */
#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniDebugDrawInput(void) {
    MiniUi *t;
    MiniInput *p;

    if (EvtGlobalFlagGet(0x5F8) != 0) {
        t = miniUi;
        p = &t->input;
        DbgPrintf(0x14, 0x10, "version %d.%.2d.%.2d", sysSaveData.unk1F69 / 100, sysSaveData.unk1F69 % 100, sysSaveData.unk1F6A);
        DbgPrintf(0x14, 0x11, "%.2x:%.2d  c:%.2d  t:%.2d  d:%.2d  s:%.2d", p->button, p->buttonWait, p->circleWait, p->triangleWait, p->crossWait, p->squareWait);
        DbgPrintf(0x18, 0x12, "pad : %.4x , %.4x , %.4x", t->input.held, p->trigger, p->repeat);
        DbgPrintf(0x18, 0x13, "l x : % 3d , z : % 3d", p->lx, p->ly);
    }
}
