/* Third-party library code (libgcc, GPL with the GCC runtime library exception): see README.md, "Third-party library code". */
#include "common.h"

typedef void (*Fn)(void);
extern Fn *__do_global_dtors_p;

void __do_global_dtors(void) {
    Fn *p;

    while (*__do_global_dtors_p != 0) {
        p = __do_global_dtors_p;
        __do_global_dtors_p = ++p;
        p[-1]();
    }
}
