/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef union 
{
  double value;
  struct 
  {
    s32 lsw;
    s32 msw;
  } parts;
} ieee_double_shape_type;
s32 isinf(double x)
{
  s32 hx;
  s32 lx;
  {
    ieee_double_shape_type ew_u;
 do { ew_u.value = x; hx = ew_u.parts.msw; lx = ew_u.parts.lsw; } while (0);
  }
  hx &= 0x7fffffff;
  hx |= ((u32) (lx | (-lx))) >> 31;
  hx = 0x7ff00000 - hx;
  return 1 - ((s32) (((u32) (hx | (-hx))) >> 31));
}
