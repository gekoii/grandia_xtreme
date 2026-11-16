#include "common.h"
#include "protos/field.h"

extern s8 chrTenantAiUnusedFlag;
extern s32 chrTenantAiUnusedWork;

void ChrTenantAiUnusedInit(void) {
    ChrTenantMoveToward(0, 0, 0, 0);
    ChrTenantAiStubRet0(0, 0, 0, 0);
    ChrTenantFindAreaPoint(0, 0, 0);
    ChrTenantAiStubPos(0);
    ChrTenantAiStubObjPos(0);
    ChrTenantAiStubVoid2(0, 0);
    ChrTenantAiStubVoid();
    chrTenantAiUnusedFlag = 0;
    chrTenantAiUnusedWork = 0;
}
