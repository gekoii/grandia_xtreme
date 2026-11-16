/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

s32 fflush(void);

s32 lflush(void) {
    return fflush();
}
