/* rodata: 0x006E2B60-0x006E2B78 */
#include "common.h"
#include "protos/battle.h"


void DbgBtlPrintOnOff(s32 x, s32 y, char *name, s32 on) {
    DbgPrintf(x, y, "%s(%s)", name, on ? "ON" : "OFF");
}
