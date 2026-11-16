/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "field/n24.h"

void VuVecCopy(void *, void *);

void MiniConcertCameraInit(void) {
    MiniConcertPos *p;

    p = &miniConcertData->pos[miniConcertWork->camFirst];
    camField[0].dist = p->dist;
    camField[0].yaw = p->yaw / 180.0f * 3.14159f;
    camField[0].pitch = p->pitch / 180.0f * 3.14159f;
    VuVecCopy(camField[0].lookAt, p);
    miniConcertWork->cam = *p;
    miniConcertWork->camFrame = 0;
}
