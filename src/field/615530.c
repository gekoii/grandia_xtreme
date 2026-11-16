#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern void GsTexPacketSet(void *, u32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void FldOverlayImgLoad(u32 arg0) {
    s32 i;
    s32 n;

    if (MapGetNo() == 0x2060 || MapGetNo() == 0xE460 || MapGetNo() == 0x2050 ||
        MapGetNo() == 0x1810 || MapGetNo() == 0x200C || MapGetNo() == 0xE450) {
        n = *(u32 *)arg0 >> 2;
        fldOverlayImg->u.s.count = 0;
        for (i = 0; i < n && ((u32 *)arg0)[i] != ((u32 *)arg0)[n - 1]; i++) {
            ((u32 *)arg0)[i] += arg0;
            GphTexDataRelocate(((u32 *)arg0)[i]);
            GsTexPacketSet(fldOverlayImg->tex[i], ((u32 *)arg0)[i], 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
            fldOverlayImg->u.s.count++;
        }
    }
}
