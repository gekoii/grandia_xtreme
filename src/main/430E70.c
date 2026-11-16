#include "common.h"
#include "main/n03.h"
#include "gph.h"

void GphMeshDrawVu0(void);

void GphVu1RenderStage2(void) {
    GphMeshDrawVu0();
    if (*(volatile s32 *)0x700000E0 != 0) {
        gphRenderCtx->unk14 += *(volatile s32 *)0x700000E0;
    } else {
        gphRenderCtx->unk1C++;
    }
    gphRenderCtx->unk18 += *(volatile s32 *)0x700000E4;
}
