/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "gph.h"

typedef struct Pkt
{
  u64 tag;
  u64 unk8;
  u32 unk10;
  u32 unk14;
  u32 unk18;
  u32 unk1C;
  u64 giftag[2];
  u64 d30;
  u64 a38;
  u64 d40;
  u64 a48;
  u64 d50;
  u64 a58;
  u64 d60;
  u64 a68;
} Pkt;
s32 fptoui(f32);
void GphPartBuildGsRegs(GphPart *obj)
{
  Pkt *p;
  u16 fl;
  u64 alpha;
  u64 tex;
  unsigned int new_var;
  u64 test;
  u64 lo;
  u64 hi;
  p = (Pkt *) (obj->gsPkt | 0x20000000);
  p->tag = 0x60000006;
  p->unk1C = 0x50000005;
  p->giftag[0] = 0x1000400000008004;
  p->giftag[1] = 0xE;
  p->unk8 = 0;
  p->unk10 = 0;
  p->unk14 = 0;
  p->unk18 = 0;
  fl = obj->gsFlags;
  alpha = 0x48;
  if (fl & 2)
  {
    alpha = 0x58;
  }
  if (fl & 0x8000)
  {
    alpha |= 0x20;
  }
  test = 0;
  if (fl & 0x10)
  {
    new_var = 0xE;
    test = ((fl >> 4) & new_var) | 0x1001;
    test |= ((u64) obj->alphaFix) << 4;
  }
  if (fl & 0x100)
  {
    test |= 0x10000;
    test |= ((u64) (fl & 0x600)) << 8;
  }
  test |= ((u64) (fl & 0x6000)) >> 1;
  tex = ((u64) (fl & 4)) >> 2;
  if (fl & 8)
  {
    tex |= 4;
  }
  lo = 0;
  hi = 0;
  switch (((u32) (fl >> 11)) & 3)
  {
    case 0:
      lo = 0x44;
      if (obj->gaObjs->unk30 != 0.0f)
    {
      lo = 0x64;
      hi = fptoui(obj->color[3] + 128.0f);
    }
    else
    {
      hi = 0x40;
    }
      break;

    case 1:
      lo = 0x29;
      hi = 0x80;
      break;

    case 2:
      lo = 0xA1;
      hi = 0x80;
      break;

  }

  lo |= hi << 32;
  p->a38 = 0x1B;
  p->d30 = alpha;
  p->d40 = test;
  p->d50 = tex;
  p->d60 = lo;
  p->a48 = 0x47;
  p->a58 = 8;
  p->a68 = 0x42;
}
