/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

unsigned int strncpy(u8 *, s32, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);
s32 sceSifSearchModuleByName(s32 arg0)
{
  u8 *p;
  if (_sceSifLoadFileInit() < 0)
  {
    return 0xFFFF0000;
  }
  if (_sceSifLoadFileVersionMismatch() != 0)
  {
    return 0xFFFEFFFC;
  }
  strncpy(_sceSifLoadBufPath, arg0, 0xFC);
  p = _sceSifLoadBufPath - 8;
  p[0x103] = 0;
  if (sceSifCallRpc(&_sceSifLoadClient, 9, 0, p, 0x200, p, 4, 0, 0) < 0)
  {
    return 0xFFFEFFFF;
  }
  return *((s32 *) (_sceSifLoadBufPath - 8));
}
