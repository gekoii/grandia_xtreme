#include "common.h"
#include "sys.h"
#include "protos/main.h"

void SysProgDrawAll(void)
{
  s32 i;
  for (i = 0; i < sysState.activeCount; i++)
  {
    sysState.program = *(sysState.active + i);
    SysProgCall(4, i, 0);
  }

}
