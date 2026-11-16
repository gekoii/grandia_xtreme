#include "common.h"
#include "main/n09.h"
#include "protos/main.h"

s32 AddIntcHandler(s32, void *, s32);
void DisableIntc(s32);
void EnableIntc(s32);
s32 GsVblankHandler(s32);
void GsVsyncInit(void)
{
  gsVblankCount = 0;
  gsVblankIdleCount = 0;
  gsFlipWaitCount = 0;
  gsTimerLastCount = 0;
  gsVblanksSinceFlip = 0;
  gsTimerTicksPerFrame = 0;
  gsVblankHandlerId = 0;
  *((volatile s32 *) 0x10000810) = 0x82;
  sceGsSyncV(0);
  *((volatile s32 *) 0x10000800) = 0;
  sceGsSyncV(0);
  gsTimerTicksPerFrame = *((volatile s32 *) 0x10000800);
  DisableIntc(2);
  gsVblankHandlerId = AddIntcHandler(2, GsVblankHandler, 0);
  EnableIntc(2);
}
