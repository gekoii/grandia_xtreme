#include "common.h"
#include "main/n06.h"

void *MemAlloc(s32 size, s32 align) {
    MemHeap *h;
    MemBlock *head;
    MemBlock *b;
    MemBlock *prev;
    MemBlock *nb;
    MemBlock *next;
    s32 sz;
    s32 d;
    u32 hs;
    MemBlock *p;
    MemBlock *q;

    hs = sizeof(MemBlock);
    h = memCurHeap;
    h->dirty = 1;
    head = h->head;
    q = (MemBlock *)h->base;
    if (head == NULL) {
        head = q;
        h->head = head;
        head->size = h->size;
        head->next = head;
    }
    b = head;
    prev = b;
    for (;;) {
        sz = b->size;
        p = (MemBlock *)((u32)b + sz - size);
        p = (MemBlock *)((u32)p & -align);
        nb = p - 1;
        d = (u32)nb - (u32)b;
        if (d >= 0) {
            if ((u32)d <= hs) {
                prev->next = b->next;
                nb = b;
            } else {
                nb->size = sz - d;
                b->size = d;
            }
            nb->heapId = memCurHeap->id;
            *((u32 *)p - 5) = (u32)nb;
            *((u32 *)p - 4) = 0xC0C0C0C0;
            *((u32 *)p - 3) = 0xC0C0C0C0;
            *((u32 *)p - 2) = 0xC0C0C0C0;
            *((u32 *)p - 1) = 0xC0C0C0C0;
            return p;
        }
        next = b->next;
        if (next == head) {
            return NULL;
        }
        prev = b;
        b = next;
    }
}
