/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

extern M2C_UNK _sceMpegSeqScalableExtStr;
extern M2C_UNK _sceMpegUnknownExtensionStr;
extern M2C_UNK _sceMpegPicSpatialScalableExtStr;
extern M2C_UNK _sceMpegPicTemporalScalableExtStr;

void _sequenceScalableExtension(void) {
    _sceMpegError(&_sceMpegSeqScalableExtStr);
}

void _unknownExtension(void) {
    _sceMpegError(&_sceMpegUnknownExtensionStr);
}

void _pictureSpatialScalableExtension(void) {
    _sceMpegError(&_sceMpegPicSpatialScalableExtStr);
}

void _pictureTemporalScalableExtension(void) {
    _sceMpegError(&_sceMpegPicTemporalScalableExtStr);
}
