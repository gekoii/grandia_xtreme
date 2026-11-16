#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "gph.h"
#include "party.h"
#include "sys.h"

extern char btlVictoryDatSuffix[];

void BtlVictoryMotionLoad(BtlUnit *u) {
    char buf[256];
    BtlVictoryWork *w;
    s32 p;
    u16 *e;

    w = &btlVictoryWork;
    MemHeapSelect(sysState.battleMagicHeap);
    switch (w->state) {
    case 1:
        if (MgcForceEnd() != 0) {
            BtlCharFileName(buf, u->def->id, btlVictoryDatSuffix);
            BtlFileLoadAsync(buf, 0);
            w->state++;
        }
        break;
    case 2:
        w->pak = BtlFileLoadAsync(NULL, 0);
        if (w->pak != 0) {
            btlVictoryWork.motionData = BtlFileRelocate(BtlPakGetEntry(w->pak, 0));
            w->motionKeys = BtlPakGetEntry(w->pak, 1);
            if (u->params->id == 0x1006) {
                p = BtlPakGetEntry(w->pak, 2);
                FileChunkRelocate(p);
                p += 0x10;
                BtlChrTexRemapToSlots(p, gphTexTableBuf, u->def->modelPack);
                e = BtlModelCreateWithPool(p, &w->unk18);
                w->prop = e;
                if (e != NULL) {
                    e[4] |= 8;
                }
            } else {
                w->prop = NULL;
            }
            BtlResultLoad(w->pak);
            w->state = 0;
        }
        break;
    }
    MemHeapSelect(sysState.battleHeap);
}
