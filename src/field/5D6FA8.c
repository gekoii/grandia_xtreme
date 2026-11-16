#include "common.h"
#include "protos/field.h"
#include "field/n16.h"


void DbgFldLinesDraw(void) {
    s32 i;
    s32 n;

    GphPrimAddGsState();
    if (dbgFldDispLines & 1) {
        EvtAreaDebugDrawAll();
    }
    if (dbgFldDispLines & 2) {
        ChrTenantDebugDrawCamTargetArea();
    }
    if (dbgFldDispLines & 4) {
        DbgFldBorderDraw(0);
    }
    if (dbgFldDispLines & 8) {
        EvtSystemFlagSet(0x70, 1);
        dbgFldPolygonCount = MapPolyDebugDraw();
    }
    if (dbgFldDispLines & 0x10) {
        DbgFldBorderDraw(1);
    }
    if (dbgFldDispLines & 0x20) {
        n = ChrTenantGetCount();
        for (i = 0; i < n; i++) {
            ChrTenantDebugDrawArea(ChrTenantAt(i));
        }
    }
    if (dbgFldDispLines & 0x40) {
        FldPointLightDebugDraw();
    }
    if (dbgFldDispLines & 0x80) {
        DbgFldTenantSearchDrawAll();
    }
    if (dbgFldDispLines & 0x100) {
        DbgFldCursorDraw();
    }
    if (dbgFldDispLines & 0x200) {
        DbgFldChrColDraw(0);
    }
    if (dbgFldDispLines & 0x400) {
        DbgFldChrColDraw(1);
    }
}
