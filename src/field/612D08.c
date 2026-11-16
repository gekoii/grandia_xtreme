#include "common.h"

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s16 e;
    s16 f;
    s32 g;
    s32 h;
} SubB;

void AmapOccSetMapJump(u8 *arg0, u16 arg1, u16 arg2) {
    SubB *s = (SubB *)(arg0 + 0x20);
    s->a = arg1;
    *(s16 *)(arg0 + 0x18) = 1;
    s->b = arg2;
    s->c = 1;
    s->d = 1;
    s->e = 0;
    s->f = 0;
    s->g = 0;
    s->h = 0;
}
