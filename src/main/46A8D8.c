#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

extern u8 movClearPacket[];
void MovClearScreen(MovDisp *t, s32 arg1)
{
  MovRect r;
  u32 addr;
  u32 new_var;
  addr = ((u32) movClearPacket) & 0x0FFFFFFF;
  r.x = 0;
  r.y = 0;
  r.w = (t->width << 1) << 3;
  r.h = 0x3800;
  new_var = addr;
  MovGsSpriteFlat(MovGsGifTag(new_var | 0x20000000, 0xE, 1, 0, 0, 0, 1, 4), &r, arg1);
  *((volatile u32 *) 0x1000A010) = addr;
  *((volatile u32 *) 0x1000A020) = 5;
  *((volatile u32 *) 0x1000A000) = 0x101;
  sceGsSyncPath(0, 0);
}
