#include "common.h"
#include "sys.h"
#include "protos/main.h"

void GsTexUpload(GsTexPacket *);

void TxtGlyphBufUpload(TxtGlyphBuf *arg0) {
    GsTexUpload(&arg0->img->texPacket);
    GsTexPacketSend(&arg0->img->texPacket);
}
