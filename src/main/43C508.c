#include "common.h"
#include "main/n06.h"

void GphPrimListClear(GphPrimList *arg0) {
    arg0->count = 0;
    arg0->entries[0].texId = 0;
}
