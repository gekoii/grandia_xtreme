/* cc: ee-gcc2.9-991111-01 */
/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"


f32 VuAcosf(f32 x) {
    if (__builtin_fabsf(x) > 1.0f) {
        return 0.0f;
    }
    return 1.570796012878418f - VuAtanf(x / sqrtf(1.0f - x * x));
}
