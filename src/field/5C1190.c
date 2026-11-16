#include "common.h"
#include "field/n14.h"

void *FldMemAlloc(s32 size);

void ChrTenantTrackPoolAlloc(s32 n) {
    ChrTenantTrackPool *p;
    void *b;

    p = FldMemAlloc(8);
    chrTenantTrackPool = p;
    p->count = n;
    if (n != 0) {
        b = FldMemAlloc(n * 0xC6);
        if (b != NULL) {
            chrTenantTrackPool->tracks = b;
            return;
        }
        chrTenantTrackPool->count = 0;
        chrTenantTrackPool->tracks = NULL;
    }
}
