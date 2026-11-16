/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0xE];
    s16 id;
    u8 pad2[0x54 - 0x10];
    s32 handle;
} Obj;


u32 __sclose(Obj *arg0) {
    return _close_r(arg0->handle, arg0->id);
}
