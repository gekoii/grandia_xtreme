#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n05.h"

extern void MemSet(s32, s32, s32);
extern void *memcpy(void *, void *, s32);

GphPart *ChrModelCreateParts(GphPart *src, s32 count)
{
  GphPart *dst;
  GphPart *end;
  GphGaObject *bs;
  GphGaObject *bd;
  s32 i;
  u16 n;
  s32 r;
  *((u8 *) 0x70003000) = 0;
  dst = chrModelCreatePartPtr;
  memcpy(dst, src, count * (sizeof(GphPart)));
  end = dst + count;
  do
  {
    dst->mtx = chrModelCreateMtxPtr;
    chrModelCreateMtxPtr += 16; /* next 0x40-byte matrix */
    dst->prio = src->prio;
    if (dst->attr != 0)
    {
      dst->attr |= 1;
    }
    if (dst->drawFlags & 1)
    {
      chrModelCreateHasDeform = 1;
    }
    if (src->gaObjs != ((void *) 0))
    {
      n = src->gaObjCount;
      bd = chrModelCreateGaObjPtr;
      chrModelCreateGaObjPtr = bd + n;
      dst->gaObjs = bd;
      bs = src->gaObjs;
      for (i = 0; i < n; i++)
      {
        if (bs->skin != NULL)
        {
          chrModelCreatePktSize += (((bs->vtxCount * 32) + 0x5F) & 0xFFFFFFC0) + 0x80;
        }
        bs->texId = 0;
        bs->vtxPkt = NULL;
        bs->deformBuf0 = NULL;
        bs->deformBuf1 = NULL;
        *(bd++) = *(bs++);
      }

      r = (s32)ChrModelPoolAlloc(0x80);
      dst->gsPkt = r;
      MemSet(r, 0, 0x80);
    }
    dst->motionSlot = 0;
    dst++;
    src++;
  }
  while (dst < end);
  return dst;
}
