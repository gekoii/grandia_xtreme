#include "common.h"
#include "sys.h"
#include "protos/field.h"


void FldUseDataHeap(void) {
    MemHeapSelect(sysDataHeap);
}
