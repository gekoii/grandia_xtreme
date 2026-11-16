#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

s32 GphScreenFadeIsActive(void) {
    return GphFadeIsActive(&gphScreenFade);
}
