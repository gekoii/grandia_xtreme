#include "common.h"
#include "field/n20.h"
#include "save.h"

void ChrPartyOrderRestore(s32 arg0, s32 arg1) {
    s32 i;

    if (chrPartyOrderBackup[0] || chrPartyOrderBackup[1] || chrPartyOrderBackup[2] || chrPartyOrderBackup[3]) {
        for (i = 0; i < 4; i++) {
            sysSaveData.party[i] = chrPartyOrderBackup[i];
        }
    }
}
