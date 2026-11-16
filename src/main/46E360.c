/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a[5];
} S0046E360;

extern S0046E360 _sceDmaEnv;

S0046E360 sceDmaGetEnv(void) {
    return _sceDmaEnv;
}
