/* Draw distance: the setting and where it is applied
   (grx_draw_distance.h, port/docs/draw-distance.md). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_draw_distance.h"
#include "grx_platform.h"

#define DD_DEFAULT 4.0f
#define DD_MAX 8.0f
/* The far clip at least this many fog distances out: the fog
   (fogMax * d^2 / (d^2 + r^2)) leaves 6% of a thing there */
#define DD_FAR_FOG 4.0f

static float dd = DD_DEFAULT;

void grx_dd_config(const char *arg) {
    float f;
    if (arg == NULL || !*arg) {
        arg = getenv("GRX_DRAW_DISTANCE");
    }
    if (arg == NULL || !*arg) {
        return;
    }
    if (sscanf(arg, "%f", &f) != 1 || f < 1.0f || f > DD_MAX) {
        grx_fatal("--draw-distance / GRX_DRAW_DISTANCE: 1 (the original) to %g, not \"%s\"", DD_MAX, arg);
    }
    dd = f;
    if (dd != DD_DEFAULT) {
        grx_log("draw distance: x%g", dd);
    }
}

float grx_draw_distance(void) {
    return dd;
}

const float *grx_dd_view(const float *c, float *tmp, float fog) {
    float far;
    if (dd == 1.0f) {
        return c;
    }
    far = c[10] * dd;
    if (far < fog * DD_FAR_FOG) {
        /* Locca's 7200 is 1.25 fog distances out, where 39% of the
           ground still shows: a hard edge, with the camera low */
        far = fog * DD_FAR_FOG;
    }
    memcpy(tmp, c, 12 * sizeof(float));
    tmp[10] = far;
    return tmp;
}

float grx_dd_fog(float dist) {
    return dist * dd;
}
