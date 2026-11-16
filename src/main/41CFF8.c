#include "common.h"
#include "protos/main.h"
#include "grxsys.h"
#include "gph.h"


void SysTexturesBind(void) {
    if (sysGrxsysPac != NULL) {
        GphTexTableSelect(gphTexTableBuf);
        GphTexRegisterAt(sysGrxsysPac->texE7[0], 0xE7);
        GphTexRegisterAt(sysGrxsysPac->texE7[1], 0xE8);
        GphTexRegisterAt(sysGrxsysPac->texE7[2], 0xE9);
        GphTexRegisterAt(sysGrxsysPac->texE7[3], 0xEA);
        GphTexRegisterAt(sysGrxsysPac->texE7[4], 0xEB);
        GphTexRegisterAt(sysGrxsysPac->texE7[5], 0xEC);
        GphTexRegisterAt(sysGrxsysPac->texE7[6], 0xED);
        GphTexRegisterAt(sysGrxsysPac->texEE, 0xEE);
        GphTexRegisterAt(sysGrxsysPac->texEF, 0xEF);
        GphTexRegisterAt(sysGrxsysPac->texF0, 0xF0);
        GphTexRegisterAt(sysGrxsysPac->texF1, 0xF1);
        GphTexRegisterAt(sysGrxsysPac->tex65[0], 0x65);
        GphTexRegisterAt(sysGrxsysPac->tex65[1], 0x66);
        GphTexRegisterAt(sysGrxsysPac->tex65[2], 0x67);
        GphTexRegisterAt(sysGrxsysPac->tex65[3], 0x68);
        GphTexRegisterAt(sysGrxsysPac->tex65[4], 0x69);
        GphTexRegisterAt(sysGrxsysPac->tex65[5], 0x6A);
        GphTexRegisterAt(sysGrxsysPac->tex65[6], 0x6B);
        GphTexRegisterAt(sysGrxsysPac->tex65[7], 0x6C);
        FilePacModelBindTextures(sysGrxsysPac->modelPackB0);
        FilePacModelBindTextures(sysGrxsysPac->modelPack148);
    }
}
