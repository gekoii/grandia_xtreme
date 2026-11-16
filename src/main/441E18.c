#include "common.h"
#include "snd.h"
#include "protos/main.h"

void SndSeStop(u8 arg0, s32 arg1)
{
  s32 idx = arg0;
  SndSeSlotShifted *e;
  s32 off;
  SndSeVoiceStop(((idx << 6) + 4) + arg1);
  off = (arg1 * 0x30) + (idx * 0xC10);
  e = (SndSeSlotShifted *) ((u8 *)sndSeSlots + off);
  if (e->state != 0)
  {
    e->flags = 0;
    e->timer = 60;
    e->wait = 0;
    e->state = 0;
  }
}
