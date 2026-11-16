#include "common.h"
#include "sys.h"
#include "main/n07.h"

void TxtFontImagePutGlyph(TxtFontImage *o, s32 idx, s32 row, s32 col)
{
  int new_var;
  s32 hw;
  s32 stride;
  u8 *base;
  u8 *src;
  u8 *dst;
  s32 *d;
  u8 *p;
  s32 i;
  if ((txtFontImageEnable == 0) || (o == ((void *) 0)))
  {
    return;
  }
  hw = o->cellW / 2;
  stride = hw * o->cellH;
  base = o->glyphSrc;
  if (o->glyphMetricsSrc != 0)
  {
    p = (u8 *) ((idx * 2) + (s32) o->glyphMetricsSrc);
    o->cellMetrics[row][col][0] = p[0];
    o->cellMetrics[row][col][1] = p[1];
  }
  dst = (o->pixels + (row * hw)) + (col * ((o->w / 2) * o->cellH));
  new_var = 0;
  i = 0;
  if (o->cellH > new_var)
  {
    src = (stride * idx) + base;
    do
    {
      d = (s32 *) (dst + ((o->w / 2) * i));
      i++;
      d[0] = ((s32 *) src)[0];
      d[1] = ((s32 *) src)[1];
      src += hw;
    }
    while (i < o->cellH);
  }
}
