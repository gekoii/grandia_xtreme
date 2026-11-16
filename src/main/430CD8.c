#include "common.h"
#include "main/n03.h"
#include "gph.h"

void GphResetSprLight(void) {
    GphRenderCtx *s = gphRenderCtx;
    s->unk40 = 0;
    s->unk44 = 0;
    s->unk48 = 0;
    s->unk4C = 128.0f;
    s->unk50 = 128.0f;
    s->unk54 = 128.0f;
    s->unk58 = 128.0f;
    s->unk5C = 128.0f;
}
