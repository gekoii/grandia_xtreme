/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"



s32 vsprintf(u8 *str, const char *fmt, void *ap) {
    s32 ret;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = _impure_ptr;
    ret = vfprintf(&f, fmt, ap);
    *f._p = 0;
    return ret;
}
