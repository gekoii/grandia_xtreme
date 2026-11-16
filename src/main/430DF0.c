#include "common.h"
#include "main/n03.h"
#include "gph.h"

extern void GphMeshDrawClip(void);

void GphVu1Render(void *arg0, void *arg1) {
    GphMeshDrawClip();
    if (*(volatile s32 *)0x700000E0 != 0) {
        gphRenderCtx->unk14 += *(s32 *)0x700000E0;
    } else {
        gphRenderCtx->unk1C++;
    }
    gphRenderCtx->unk18 += *(volatile s32 *)0x700000E4;
}
