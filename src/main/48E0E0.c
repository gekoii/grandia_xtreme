/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "m2c_macros.h"


M2C_UNK *sceSifGetDataTable(void) {
    return (M2C_UNK *)&_sceSifCmdData;
}
