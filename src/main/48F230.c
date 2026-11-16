/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"

volatile int DIntr(void);
void EIntr(void);
void sceSifSetRpcQueue(sceSifQueueData *n, s32 arg1)
{
  sceSifQueueData *p;
  sceSifQueueData *q;
  DIntr();
  n->key = arg1;
  n->active = 0;
  n->link = 0;
  n->start = 0;
  n->end = 0;
  n->next = (void *) 0;
  if (_sceSifRpcData.active_queue == ((void *) 0))
  {
    _sceSifRpcData.active_queue = n;
  }
  else
  {
    for (p = _sceSifRpcData.active_queue; p->next != ((void *) 0); p = p->next)
    {
    }

    p->next = n;
  }
  EIntr();
}
