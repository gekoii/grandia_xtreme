/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "m2c_macros.h"

M2C_UNK sceSifAddCmdHandler(M2C_UNK, M2C_UNK *, M2C_UNK); /* extern */
M2C_UNK DIntr();                            /* extern */
M2C_UNK EIntr();                            /* extern */
extern M2C_UNK _sceCdPOffHandler;

s32 _sceCdPOffInit(void) {
    _sceCdInitBusy = 1;
    DIntr();
    sceSifAddCmdHandler(0x80000012, &_sceCdPOffHandler, 0);
    EIntr();
    _sceCdInitBusy = 0;
    _sceCdSifCmdBind = 1;
    return 1;
}
