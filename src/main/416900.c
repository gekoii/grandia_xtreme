#include "common.h"
#include "party.h"




void PtyCharStoreUnitStats(PtyUnit *arg0) {
PtyChar *e = &ptyCharData[arg0->id - 0x1001];
    e->exp = arg0->exp;
    e->hp = arg0->hp;
    e->sp = arg0->sp;
    e->status = arg0->status;
}
