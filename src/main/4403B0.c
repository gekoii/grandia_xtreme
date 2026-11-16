#include "common.h"
#include "protos/main.h"
#include "main/n06.h"

s32 MemHeapReset(s32 arg0)
{
  MemHeap *h = MemHeapFind(arg0);
  MemBlock *n = (MemBlock *)h->base;
  MemBlock *new_var3;
  MemHeap *new_var;
  MemBlock *p;
  MemBlock *new_var2;
  new_var = h;
  new_var->dirty = 0;
  n->size = 0;
  p = n + 1;
  n->next = p;
  n = n->next;
  new_var2 = n;
  p->next = new_var2 - 1;
  new_var3 = p;
  new_var3->size = h->size - 0x40;
  return 0;
}
