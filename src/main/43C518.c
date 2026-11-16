#include "common.h"
#include "main/n06.h"

void GphPrimListSetTexture(s32 arg0) {
    if (gphPrimList != NULL) {
        if (gphPrimList->count < 0x3FF) {
            gphPrimList->entries[gphPrimList->count].texId = arg0;
        }
    }
}
