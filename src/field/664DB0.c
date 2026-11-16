#include "common.h"

typedef struct {
    u16 a, b, c, d, e;
} Rec664DB0;

extern Rec664DB0 evtMsgLayoutTable[];

void EvtMsgLayoutSet(Rec664DB0 *src, s32 idx) {
    if (idx < 7) {
        evtMsgLayoutTable[idx].a = src->a;
        evtMsgLayoutTable[idx].b = src->b;
        evtMsgLayoutTable[idx].c = src->c;
        evtMsgLayoutTable[idx].d = src->d;
        evtMsgLayoutTable[idx].e = src->e;
    }
}
