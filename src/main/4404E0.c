#include "common.h"
#include "main/n06.h"

u32 MemGetLargestFree(void) {
    MemBlock *head;
    MemBlock *p;
    MemBlock *next;
    u32 max;
    u32 v;

    if (memCurHeap == NULL) {
        return 0;
    }
    head = memCurHeap->head;
    max = head->next->size - 0x20;
    p = head->next->next;
    if (p != head) {
        do {
            next = p->next;
            v = p->size - 0x20;
            p = next;
            if (max < v) {
                max = v;
            }
        } while (next != head);
    }
    return max;
}
