#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"


ChrObj *ChrGetByPartySlot(s32 n) {
    return ChrGetByNo(sysSaveData.party[n - 1]);
}
