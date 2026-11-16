#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

s32 BtlFileLoadAsync(char *name, s32 buf) {

    if (btlFileLoadState == 2) {
        if (SndSeBankLoadGetState() == -1) {
            btlFileLoadState = 0;
            return btlFileLoadBuf;
        }
    } else if (btlFileLoadState == 0) {
        if (buf == 0) {
            if (name == NULL) {
                return 0;
            }
            btlFileLoadBuf = MemAlloc128(FileGetSizeAligned(name));
        } else {
            btlFileLoadBuf = buf;
        }
        FileReadStart(name, btlFileLoadBuf);
        btlFileLoadedCallback = NULL;
        btlFileLoadState = 1;
    } else {
        if (FileGetReadState() != -1) {
            return 0;
        }
        if (btlFileLoadedCallback == NULL) {
            btlFileLoadState = 0;
            return btlFileLoadBuf;
        }
        btlFileLoadedCallback();
        btlFileLoadState = 2;
    }
    return 0;
}
