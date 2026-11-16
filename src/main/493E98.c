/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a;
    s32 b;
} Args;

s32 Deci2Call(s32, Args *);

s32 sceDeci2ReqSend(s32 arg0, s8 arg1, u8 *arg2) {
    Args s;
    s.a = arg0;
    s.b = arg1;
    return Deci2Call(3, &s);
}
