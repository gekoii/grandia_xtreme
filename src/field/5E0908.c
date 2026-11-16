#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void GsTexUpload(void *);

void EffDrawBindTex1Model6(void) {
    GsRenderStatePacketSend(effMapWork->drawEnvs[1]);
    GsTexUpload(&effMapWork->texObjs[6]);
    GsTexPacketSend(&effMapWork->texObjs[6]);
}
