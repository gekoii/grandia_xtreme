/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"

s32 _localeconv_r(struct _reent *);

s64 localeconv(void) {
    return _localeconv_r(_impure_ptr);
}
