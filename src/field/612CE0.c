#include "common.h"

typedef struct {
    s16 a;
    s16 b;
    s32 c;
    s32 d;
    s32 e;
} Sub20;

void AmapOccSetScript(u8 *arg0, u16 arg1) {
    Sub20 *s = (Sub20 *)(arg0 + 0x20);
    *(s16 *)(arg0 + 0x20) = arg1;
    *(s16 *)(arg0 + 0x18) = 0;
    s->b = 1;
    s->d = 0;
    s->e = 0;
    s->c = 0;
}
