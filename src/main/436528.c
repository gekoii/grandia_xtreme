#include "common.h"
#include "main/n04.h"

void UtilSortU64(u64 *, u32, void *);
void GphDrawListSort(void)
{
  GphDrawList *h;
  u64 *list;
  u32 *tbl;
  u32 cnt;
  u32 i;
  u64 *new_var;
  u32 j;
  u32 prev;
  u32 key;
  h = gphDrawListCur;
  list = h->keys;
  tbl = h->layerStart;
  cnt = h->count;
  UtilSortU64(list, cnt, (void *) 0x70003000);
  h->layerStart[0] = 0;
  prev = 0;
  key = 0;
  for (i = 0; i < cnt; i++)
  {
    new_var = &list[i];
    key = (*new_var) >> 60;
    if (prev < key)
    {
      for (j = prev + 1; j <= key; j++)
      {
        tbl[j] = i;
      }

      prev = key;
    }
  }

  for (i = key + 1; i < 16; i++)
  {
    tbl[i] = cnt;
  }

}
