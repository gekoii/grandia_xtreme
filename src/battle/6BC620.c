/* rodata: 0x006E1A90-0x006E1AB0 */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"
#include "battle/env.h"
#include "battle/misc.h"
#include "grxsys.h"
#include "sys.h"

typedef void (*EntFn)(BtlCharDef *);


void BtlDataLoad(void) {
    BtlCharDef *e;
    s32 i;

    EnvStageLoad();
    e = btlCharDefs;
    for (i = 0; i < 8; i++, e++) {
        if (e->id != 0) {
            e->setState = BtlUnitStateSetHandler;
            e->init = 0;
            e->attack = 0;
            e->validate = 0;
            if (i < 4) {
                BtlCharDefSetup(e, i);
                if (btlHandoff.partyLoaded[i] != 0) {
                    EntFn fn[4] = { (EntFn)0x708000, (EntFn)0x710000, (EntFn)0x718000, (EntFn)0x720000 };
                    fn[i](e);
                }
            } else {
                BtlCharDefSetup(e, i - 4);
                if (btlHandoff.partyLoaded[i] != 0) {
                    EntFn fn[4] = { (EntFn)0x728000, (EntFn)0x730000, (EntFn)0x738000, (EntFn)0x740000 };
                    fn[i - 4](e);
                }
            }
        }
    }
    GphTexRegisterAt(sysGrxsysPac->tex65[0], 0x65);
    GphTexRegisterAt(sysGrxsysPac->tex65[1], 0x66);
    GphTexRegisterAt(sysGrxsysPac->tex65[2], 0x67);
    GphTexRegisterAt(sysGrxsysPac->tex65[3], 0x68);
    GphTexRegisterAt(sysGrxsysPac->tex65[4], 0x69);
    GphTexRegisterAt(sysGrxsysPac->tex65[5], 0x6A);
    GphTexRegisterAt(sysGrxsysPac->tex65[6], 0x6B);
    GphTexRegisterAt(sysGrxsysPac->tex65[7], 0x6C);
    GphTexRegisterAt(sysGrxsysPac->texE7[0], 0xE7);
    GphTexRegisterAt(sysGrxsysPac->texE7[1], 0xE8);
    GphTexRegisterAt(sysGrxsysPac->texE7[2], 0xE9);
    GphTexRegisterAt(sysGrxsysPac->texE7[3], 0xEA);
    GphTexRegisterAt(sysGrxsysPac->texE7[4], 0xEB);
    GphTexRegisterAt(sysGrxsysPac->texE7[5], 0xEC);
    GphTexRegisterAt(sysGrxsysPac->texE7[6], 0xED);
    GphTexRegisterAt(sysGrxsysPac->texF8[0], 0x34);
    GphTexRegisterAt(sysGrxsysPac->texF8[1], 0x36);
    GphTexRegisterAt(sysGrxsysPac->texF8[2], 0x37);
    GphTexRegisterAt(sysGrxsysPac->tex11C[0], 0x35);
    GphTexRegisterAt(sysGrxsysPac->tex11C[1], 0x3F);
    GphTexRegisterAt(sysGrxsysPac->tex11C[2], 0x40);
    GphTexRegisterAt(sysGrxsysPac->tex11C[3], 0x3E);
    GphTexRegisterAt(sysGrxsysPac->tex11C[4], 0x33);
    GphTexRegisterAt(sysGrxsysPac->tex11C[5], 0xEE);
    GphTexRegisterAt(sysGrxsysPac->tex140, 0x41);
    GphTexRegisterAt(sysGrxsysPac->texEF, 0xEF);
    GphTexRegisterAt(sysGrxsysPac->texF0, 0xF0);
    GphTexRegisterAt(sysGrxsysPac->texF1, 0xF1);
    GphTexRegisterAt(sysGrxsysPac->tex44, 0x42);
    btlSysModelPack = sysGrxsysPac->modelPackE8;
    FilePacModelBindTextures(btlSysModelPack);
    envParamTable = sysGrxsysPac->envParamOfs;
    btlVoiceTable = sysGrxsysPac->unk14C;
    FxHitLoad();
    PtclBurstLoad();
    FxUnitLoad();
    FxAtkLoad();
    BtlCmdMenuLoad();
    btlCharDefFinishIndex = 0;
    btlCharDefFinishBusy = 0;
}
