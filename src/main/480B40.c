/* Matches libgcc's __do_global_ctors (libgcc2.c / gbl-ctors.h). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

typedef void (*func_ptr)(void);

extern func_ptr __CTOR_LIST__[];

#define DO_GLOBAL_CTORS_BODY                                            \
    do {                                                                \
        unsigned long nptrs = (unsigned long)__CTOR_LIST__[0];             \
        unsigned i;                                                     \
        if (nptrs == (unsigned long)-1)                                 \
            for (nptrs = 0; __CTOR_LIST__[nptrs + 1] != 0; nptrs++);       \
        for (i = nptrs; i >= 1; i--)                                    \
            __CTOR_LIST__[i]();                                            \
    } while (0)

void __do_global_ctors(void) {
    DO_GLOBAL_CTORS_BODY;
}
