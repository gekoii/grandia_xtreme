/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "sdk/eekernel.h"
#include "protos/main.h"

extern s32 CreateSema(SemaParam *);
extern s32 DeleteSema(s32);
extern s32 WaitSema(s32);
extern void sceSifWriteBackDCache(void *, s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);
s32 sceMount(char *name1, char *name2, s32 arg, u8 *data, s32 len)
{
  SemaParam sema;
  s32 result;
  SceFsMountReq *req;
  s32 sid;
  s32 r;
  s32 i;
  req = &_sceFsSendBuf.mount;
  _sceFsLock(0x14);
  if (_sceFsInitDone == 0)
  {
    sceFsInit();
  }
  for (i = 0; i < 0x400; i++)
  {
    if ((req->name1[i] = name1[i]) == 0)
    {
      break;
    }
  }

  if (i == 0x400)
  {
    req->name1[0x3FF] = 0;
  }
  for (i = 0; i < 0x400; i++)
  {
    if ((req->name2[i] = name2[i]) == 0)
    {
      break;
    }
  }

  if (i == 0x400)
  {
    req->name2[0x3FF] = 0;
  }
  if (len > 0x400)
  {
    _sceFsUnlock();
    return -7;
  }
  for (i = 0; i < len; i++)
  {
    req->arg[i] = data[i];
  }

  req->argLen = len;
  req->flag = arg;
  sema.maxCount = 1;
  sema.initCount = 0;
  sema.option = 0;
  req->sema = (sid = CreateSema(&sema));
  req->result = &result;
  req->resultSize = 4;
  sceSifWriteBackDCache(&_sceFsSendBuf, sizeof(SceFsMountReq));
  if (sceSifCallRpc(&_sceFsClient, 0x14, 0, &_sceFsSendBuf, sizeof(SceFsMountReq), _sceFsRecvBuf, 4, (void *) 0, (void *) 0) < 0)
  {
    DeleteSema(sid);
    _sceFsUnlock();
    return -0xB;
  }
  r = *((s32 *) (((u32) _sceFsRecvBuf) | 0x20000000));
  _sceFsUnlock();
  if (r == 0)
  {
    DeleteSema(sid);
    return -0xB;
  }
  WaitSema(sid);
  DeleteSema(sid);
  return result;
}
