#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "save.h"
#include "sys.h"
#include "field/n17.h"

void MemSet(void *, s32, s32);

void PtyPartySet(s16 *ids) {
    s32 i;
    s32 n = 0;
    s16 id;

    PtyPartyClear();
    for (i = 0; i < 4; i++) {
        MemSet(&ptyPartyUnits[n], 0, sizeof(PtyUnit));
        btlHandoff.unitIds[i] = 0;
        btlHandoff.partyKind[i] = 0;
        sysSaveData.party[i] = 0;
        id = ids[i];
        if (id != 0) {
            btlHandoff.partyKind[n] = id + 0x1000;
            btlHandoff.partyModels[n] = (ChrPackBinding *)((id - 1) * 0x2C + (u32)fldPartyPackTable);
            btlHandoff.unitIds[n] = id + 0x1000;
            btlHandoff.partyLoaded[n] = 1;
            btlHandoff.unitFlags[n] = 1;
            sysSaveData.party[n] = id;
            PtyUnitInitFromChar(&ptyPartyUnits[n], id + 0x1000);
            PtyUnitCalcParams(&ptyPartyUnits[n], 0, 0);
            n++;
        }
    }
}
