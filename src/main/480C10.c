/* as: ee-gcc2.9-991111 */
/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

s64 __fixunssfdi(f32);

s64 __fixsfdi(f32 x) {
    if (x < 0.0f) {
        return -__fixunssfdi(-x);
    }
    return __fixunssfdi(x);
}
