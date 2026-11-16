#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/state.h"

void GsTexUpload(GsTexPacket *);

void TxtWinTextureBind(u8 n) {
    if (n >= 3) {
        GsTexUpload(&txtWinTexturesBase[n].tex);
        GsTexPacketSend(&txtWinTexturesBase[n].tex);
    } else {
        GsTexUpload(&txtWinTextures[n].tex);
        GsTexPacketSend(&txtWinTextures[n].tex);
    }
}
