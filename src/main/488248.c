/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

void _fwalk(s32, void *);
void fflush(void);

void _cleanup_r(s32 arg0) {
    _fwalk(arg0, fflush);
}
