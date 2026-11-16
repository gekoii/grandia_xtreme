/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 data[0x40];
} E;

typedef struct {
    u8 pad[0x1C];
    E *items;
    s32 count;
} T;

E *_rpc_get_fpacket(T *, s32);

E *_rpc_get_fpacket2(T *arg0, s32 arg1) {
    if (arg1 < 0 || arg1 >= arg0->count) {
        return _rpc_get_fpacket(arg0, arg1);
    }
    return &arg0->items[arg1];
}
