/* as: ee-gcc2.9-991111-01 */
#include "common.h"

void OccWrapAngles3(s32 *a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (a[i] > 0.0f) {
            while (a[i] > 32768.0f) {
                a[i] -= 65536.0f;
            }
        } else {
            while (a[i] < -32768.0f) {
                a[i] += 65536.0f;
            }
        }
    }
}
