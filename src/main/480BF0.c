/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern s32 __main_initialized;


void __main(void) {
    if (__main_initialized == 0) {
        __main_initialized = 1;
        __do_global_ctors();
    }
}
