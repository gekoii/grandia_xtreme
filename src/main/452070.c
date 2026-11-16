/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "main/n07.h"

void GphFadeUpdate(GphFade *a) {
    if (a->time <= a->count) {
        a->active = 0;
        return;
    }
    a->color[0] += a->delta[0];
    a->color[1] += a->delta[1];
    a->color[2] += a->delta[2];
    a->color[3] += a->delta[3];
    a->count += 1.0f;
}
