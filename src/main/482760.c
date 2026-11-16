/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"
#include "m2c_macros.h"

/* newlib's _impure_ptr (struct _reent *); _errno is its first field */

/* newlib __errno(): &_impure_ptr->_errno */
s32 *__errno(void) {
    return &_impure_ptr->_errno;
}
