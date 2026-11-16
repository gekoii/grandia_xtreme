#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

void MemSet(void *, s32, s32);

s32 FldMenuNumberWidth(s32 arg0, s32 arg1, f32 scale) {
    u8 buf[8];
    s32 i;
    s32 sum;

    MemSet(buf, 0, 8);
    i = UtilIntToDigits(arg0, buf);
    sum = 0;
    for (; i < 8; i++) {
        sum += (s32)(menuDigitFont.w[buf[i]] * scale);
    }
    return sum;
}
