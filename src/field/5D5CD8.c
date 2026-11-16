#include "common.h"
#include "protos/field.h"
#include "field/n16.h"

void DbgFldInit(void) {
    dbgMusicSeek = 1;
    dbgFldDispLines = 0;
    dbgFldPolygonCount = 0;
    dbgFldMoveCursorPos.x = 0.0f;
    dbgFldMoveCursorDir = 0.0f;
    dbgFldMoveCursorPos.y = 0.0f;
    dbgFldMoveCursorPos.z = 0.0f;
    dbgFldMoveCursorPos.w = 0.0f;
    SndBgmSetSeek(dbgMusicSeek);
}
