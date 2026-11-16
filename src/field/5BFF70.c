/* as: ee-gcc2.9-991111-01 */
#include "common.h"

void OccWrapDegrees3(f32 *a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (a[i] > 0.0f) {
            while (a[i] > 180.0f) {
                a[i] -= 360.0f;
            }
        } else {
            while (a[i] < -180.0f) {
                a[i] += 360.0f;
            }
        }
    }
}
