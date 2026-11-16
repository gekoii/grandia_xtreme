#include "common.h"
#include "gph.h"
#include "main/n04.h"

void GphDrawListIndex(void)
{
  GphDrawList *s;
  u32 prev;
  u32 k;
  u32 i;
  u32 n;
  u32 j;
  u32 *tbl;
  u64 *new_var;
  u64 *items;
  s = gphDrawListCur;
  prev = 0;
  k = 0;
  i = 0;
  n = s->count;
  tbl = s->layerStart;
  items = s->keys;
  tbl[0] = 0;
  for (; i < n; i++)
  {
    new_var = &(*(items + i));
    k = (*new_var) >> 60;
    if (prev < k)
    {
      for (j = prev + 1; j <= k; j++)
      {
        tbl[j] = i;
      }

      prev = k;
    }
  }

  for (i = k + 1; i < 16; i++)
  {
    tbl[i] = n;
  }

}
