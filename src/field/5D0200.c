#include "common.h"
#include "protos/field.h"
#include "camera.h"
#include "field/n15.h"

void CamScrSetPos(void *arg0, s32 arg1, s32 arg2) {
    f32 temp_f0;

    temp_f0 = CamScrArgFloat(arg2);
    if (arg2 != CAM_SCR_KEEP) {
        switch (arg1) {                             /* irregular */
        case 0:
            ((Camera *)arg0)->eye[0] = temp_f0;
            break;
        case 1:
            ((Camera *)arg0)->eye[1] = temp_f0;
            break;
        case 2:
            ((Camera *)arg0)->eye[2] = temp_f0;
            break;
        }
    }
}
