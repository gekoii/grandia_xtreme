#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"

void GsTexUpload(void *);

void EvtFaceWinDraw(void) {
    s32 i;

    if (fldState.mode == 1) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (evtFaceWins[i].active != 0) {
            if (ChrPortraitExists(evtFaceWins[i].id) == 1) {
                ChrPortraitSet(i, evtFaceWins[i].id);
                GsTexUpload((void *)ChrPortraitGetTex(i));
                GsTexPacketSend(ChrPortraitGetTex(i));
                GphBatchDrawSprites(&evtFaceWins[i].spr);
            }
            GsTexUpload(txtMsgWinTexture);
            GsTexPacketSend((s32)txtMsgWinTexture);
            GphBatchDrawQuads(evtFaceWins[i].quads);
        }
    }
}
