#include "common.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "sys.h"
#include "protos/battle.h"

void EnvFlareInit(void) {
    EnvParam *bg;

    envFlareAlpha = 1.0f;
    envFlareAlphaStep = 0;
    bg = EnvParamGet(btlEntryMapNo);
    envFlareIntensity = bg->flareIntensity;
    envFlareDistance = bg->flareDist;
    envFlareAngleX = bg->flareAngleX;
    envFlareAngleY = bg->flareAngleY;
    EnvFlareCalcPos(bg);
}
