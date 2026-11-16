#include "common.h"
#include "main/n03.h"
#include "gph.h"

void GphMeshDrawNoClip(void);

#define SPR_E0 (*(volatile s32 *)0x700000E0)
#define SPR_E4 (*(volatile s32 *)0x700000E4)
#define CNT gphRenderCtx

void GphVu1RenderStage3(void) {
    GphMeshDrawNoClip();
    if (SPR_E0 != 0) {
        CNT->unk14 += SPR_E0;
    } else {
        CNT->unk1C++;
    }
    CNT->unk18 += SPR_E4;
}
