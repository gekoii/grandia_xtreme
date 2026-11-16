#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

s32 McSaveMakeDir(s32 arg0) {
    s32 a1;
    s32 n;
    McReq *p;

    if (arg0 == 0) {
        mcSaveMkdirReq = McReqMkdir((s32)mcSaveFlowDirPath);
        return 0;
    }
    if (McIsBusy() != 0) {
        return 0;
    }
    McGetCurPort();
    n = McReqGetStatus(mcSaveMkdirReq);
    p = McReqGet(mcSaveMkdirReq);
    McReqFreeId(mcSaveMkdirReq);
    if (n > 0) {
        a1 = p->result;
        if (a1 >= 0) {
            mcDirListPtr->count = a1;
            return 1;
        }
    }
    return -1;
}
