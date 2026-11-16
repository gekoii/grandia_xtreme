#include "common.h"
#include "protos/field.h"
#include "field/state.h"

void GsTexUpload(void *);

void EffDrawBindTex0Model5(void) {
    GsRenderStatePacketSend(effMapWork->drawEnvs[0]);
    GsTexUpload(&effMapWork->texObjs[5]);
    GsTexPacketSend(&effMapWork->texObjs[5]);
}
