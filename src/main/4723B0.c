/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

void DIntr(void);
volatile unsigned long EIntr(void);
/* libmpeg declares int sceMpegInit(void); nothing here sets the result */
s32 sceMpegInit(void)
{
  DIntr();
  *((volatile u32 *) 0x1000F590) = (*((volatile u32 *) 0x1000F520)) | 0x10000;
  *((volatile u32 *) 0x1000B000) &= 0xFFFFFEFF;
  *((volatile u32 *) 0x1000B400) &= 0xFFFFFEFF;
  *((volatile u32 *) 0x1000F590) = (*((volatile u32 *) 0x1000F520)) & 0xFFFEFFFF;
  EIntr();
  {
    volatile u32 *a = (volatile u32 *) 0x1000B020;
    volatile u32 *c = (volatile u32 *) 0x1000B420;
    *a = 0;
    *c = 0;
  }
  sceIpuInit();
}
