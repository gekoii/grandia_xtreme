#include "common.h"
#include "main/n03.h"
#include "gph.h"
#include "camera.h"

void CamSetCurrent(Camera *arg0) {
    gphRenderCtx->camera = arg0;
}
