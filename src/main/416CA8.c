#include "common.h"
#include "party.h"
void PtyManaEggsAddMp(PtyChar *arg0, s32 arg1, s32 arg2)
{
  u16 new_var;
  s32 i;
  u16 id;
  PtyManaEgg *e;
  s16 v;
  for (i = 0; i < arg2; i++)
  {
    id = arg0->slots[i];
    if (1)
    {
      new_var = id;
      if (new_var < 24)
      {
        e = &ptyPartyData->eggs[(s16) id];
        v = (e->mp = e->mp + arg1);
        e->mp = (new_var = (v < 0) ? (0) : ((e->maxMp < v) ? (e->maxMp) : (e->mp)));
      }
    }
  }

}
