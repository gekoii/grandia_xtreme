/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"



s32 _vfprintf_r(struct _reent *, FILE *, s32, s32);

s32 vfprintf(FILE *obj, s32 arg1, s32 arg2) {
    do {
        if (obj->_data == NULL) {
            obj->_data = _impure_ptr;
        }
    } while (0);
    if (obj->_data->__sdidinit == 0) {
        __sinit(obj->_data);
    }
    return _vfprintf_r(obj->_data, obj, arg1, arg2);
}
