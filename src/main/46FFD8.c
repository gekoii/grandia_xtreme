/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "m2c_macros.h"


M2C_UNK *_sceMcGetRpcClient(M2C_UNK **arg0, M2C_UNK **arg1) {
    *arg0 = (M2C_UNK *)&_sceMcRecvBuf;
    *arg1 = (M2C_UNK *)&_sceMcCmdNo;
    _sceMcRecvBuf.sema = _sceMcSema;
    return (M2C_UNK *)&_sceMcClient;
}
