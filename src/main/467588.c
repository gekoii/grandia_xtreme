#include "common.h"
#include "snd.h"

s32 DisableIntc(s32);
void EnableIntc(s32);
void DisableDmac(s32);
s32 EnableDmac(s32);

void voBufIncCount(VoBuf *arg0) {
    DisableDmac(2);
    DisableIntc(2);
    arg0->tag[arg0->write]->status = 2;
    arg0->count++;
    arg0->write = (arg0->write + 1) % arg0->size;
    EnableDmac(2);
    EnableIntc(2);
}
