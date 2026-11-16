#include "common.h"
#include "main/n02.h"

void DbgSetClearColor(u8 arg0, u8 arg1, u8 arg2) {
    dbgClearR = arg0;
    dbgClearG = arg1;
    dbgClearB = arg2;
}

void DbgSetCamera(struct Camera *arg0) {
    dbgCamera = arg0;
}

s8 DbgGetObjNo(void) {
    return dbgObjNo;
}
