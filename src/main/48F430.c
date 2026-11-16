/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"



extern s32 DIntr(void);
extern void EIntr(void);

sceSifQueueData *sceSifRemoveRpcQueue(sceSifQueueData *arg0) {
    sceSifQueueData *p;

    DIntr();
    p = _sceSifRpcData.active_queue;
    if (p == arg0) {
        _sceSifRpcData.active_queue = p->next;
    } else {
        for (; p != NULL; p = p->next) {
            if (p->next == arg0) {
                p->next = arg0->next;
                break;
            }
        }
    }
    EIntr();
    return p;
}
