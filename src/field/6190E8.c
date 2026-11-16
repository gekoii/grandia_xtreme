/* rodata: 0x00674E10-0x00674E60 */
#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

void FldMemDebugPrint(void) {
    DbgPrintf(0x1C, 0xF, "TOP  %08x", fldMemTop);
    DbgPrintf(0x1C, 0x10, "END  %08x", fldMemEnd);
    DbgPrintf(0x1C, 0x11, "SIZE %08x %dM", fldMemEnd - fldMemTop, (fldMemEnd - fldMemTop) >> 20);
    DbgPrintf(0x1C, 0x12, "USE  %08x %dM", fldMemCur - fldMemTop, (fldMemCur - fldMemTop) >> 20);
    DbgPrintf(0x1C, 0x13, "FREE %08x %dM", fldMemEnd - fldMemCur, (fldMemEnd - fldMemCur) >> 20);
}
