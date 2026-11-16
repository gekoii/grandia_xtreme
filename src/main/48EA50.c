/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
typedef struct { u8 pad[0x40]; } E0048EA50;
typedef struct { u8 pad[0x14]; E0048EA50 *base; s32 count; s32 pad2[2]; s32 idx; } S0048EA50;
E0048EA50 *_rpc_get_fpacket(S0048EA50 *arg0) {
    s32 i = arg0->idx % arg0->count; E0048EA50 *r = &arg0->base[i]; arg0->idx = i + 1; return r;
}
