/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 a;
    s32 b;
} S;

void Deci2Call(s32, S *);

void sceDeci2ExReqSend(s32 arg0, s8 arg1) {
    S s;
    s.a = arg0;
    s.b = arg1;
    Deci2Call(-7, &s);
}
