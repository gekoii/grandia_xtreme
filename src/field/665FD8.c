/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/field.h"


void exit(s32 arg0) {
    __do_global_dtors();
    _cleanup();
    _exit(arg0);
}
