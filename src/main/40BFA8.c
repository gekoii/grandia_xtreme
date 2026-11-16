#include "common.h"
#include "protos/main.h"
#include "gph.h"


void UtilTexRegisterWithId(GphTex *arg0, u64 arg1) {
    if (arg1 != 0) {
        arg0->id = arg1;
    }
    GphTexDataRelocate(arg0);
    GphTexRegisterAt(arg0, (u8)arg1);
}
