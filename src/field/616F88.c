#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"

void FldSndStreamPlay(u32 id) {
    s32 ofs;
    s32 base;
    s32 mask;
    s32 i;

    ofs = 0x40;
    mask = 0;
    if (id == 0x38E || id == 0x390 || id == 0x392 || id == 0x394 || id == 0x396) {
        switch (id) {
        case 0x390:
            ofs = 0x80;
            break;
        case 0x392:
            ofs = 0x88;
            break;
        case 0x394:
            ofs = 0x90;
            break;
        case 0x38E:
            ofs = 0x98;
            break;
        case 0x396:
            ofs = 0xA0;
            break;
        }
        base = ofs + 0x400;
        for (i = 0; i < 8; i++) {
            s32 r = EvtGlobalFlagGet(base + i);
            if (r == 1) {
                mask |= r << i;
            }
        }
        if (id != 0xFFFFFFFF) {
            fldSndStreamId = id;
        }
        {
            s32 r = SndStreamPlayTracks(id, mask);
            if (r >= 0) {
                fldSndStreamHandle = r;
            }
        }
    } else {
        if (id != 0xFFFFFFFF) {
            fldSnd.strmId = id;
        }
        {
            s32 r = SndStreamPlay(fldSnd.strmId, 0, 0xFF);
            if (r >= 0) {
                fldSnd.strmHandle = r;
            }
        }
    }
    EvtSystemFlagSet(0x8B, 1);
}
