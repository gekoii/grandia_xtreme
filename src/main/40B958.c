#include "common.h"
#include "camera.h"

/* lookAt[0] is stored as an int 0 and read back as a float through CamFU
 * (camera.h): the load has to stay after the store. */
void CamDebugReset(Camera *a) {
    ((CamFU *)a->lookAt)->i = 0;
    a->lookAt[1] = 0.0f;
    a->lookAt[2] = 0.0f;
    a->pitch = a->yaw = a->roll = ((CamFU *)a->lookAt)->f;
    a->dist = 3000.0f;
    a->mode = CAM_MODE_NORMAL;
}
