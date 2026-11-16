#include "common.h"
#include "protos/main.h"
#include "main/n05.h"

void GsTexUpload(GphEnvMap *);

void GphEnvMapSendTex(s32 arg0) {
    GphEnvMap *e = &gphEnvMapTable[arg0];
    GsTexUpload(e);
    GsTexPacketSend((s32)e);
    gphRenderCtx->texUploads++;
    gphRenderCtx->texUploadSize += e->texW * e->texH;
}
