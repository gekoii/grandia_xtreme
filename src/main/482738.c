/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 atoi(s32 arg0) {
    return strtol(arg0, 0, 10);
}
