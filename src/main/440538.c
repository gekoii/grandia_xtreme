#include "common.h"
#include "main/n06.h"

s32 MemGetFreeTotal(void) {
    MemBlock *head;
    MemBlock *first;
    MemBlock *p;
    MemBlock *next;
    s32 sum;

    if (memCurHeap == NULL) {
        return 0;
    }
    head = memCurHeap->head;
    first = head->next;
    sum = first->size - 0x20;
    p = first->next;
    if (p != head) {
        do {
            s32 t = p->size - 0x20;
            next = p->next;
            sum += t;
            p = next;
        } while (next != head);
    }
    return sum;
}
