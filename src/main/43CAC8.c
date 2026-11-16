#include "common.h"
#include "protos/main.h"

void GphLineVertsProject(u32, u64 *, void *);
void GphLine3DDraw(u32 n, void *data, u32 tex, s32 flag)
{
  u32 half;
  u32 qwc;
  int new_var;
  u64 *q;
  u64 *p;
  half = (n + 1) >> 1;
  qwc = half + 4;
  q = GsPacketAlloc64((half + 5) * 16);
  if (q != ((void *) 0))
  {
    q[1] = 0;
    ((u32 *) q)[4] = 0;
    ((u32 *) q)[5] = 0;
    ((u32 *) q)[6] = 0;
    q[0] = (u32) (qwc | 0x10000000);
    qwc = half + 3;
    ((u32 *) q)[7] = (qwc & 0xFFFF) | 0x50000000;
    q[5] = 0x10;
    q[4] = 0x2400000000000001;
    new_var = (flag & 1) ? (2) : (1);
    q[6] = new_var;
    q[7] = ((u64) tex) | 0x3F80000000000000;
    q[9] = 4;
    q[8] = ((u64) (n | 0x8000)) | 0x1400000000000000;
    p = q + 10;
    GphLineVertsProject(n, p, data);
    p += n;
    if (n & 1)
    {
      *p = 0;
    }
    GphPrimListAdd(q);
  }
}
