/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void Exit(s32);

void _exit(s32 arg0) {
    Exit(0);
}
