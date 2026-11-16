/* Port-only C for data the decompilation still has in assembly (asm/main/data/main/0A7304.rodata.s). */
#include "common.h"

const char _sceMpegSecondFieldMissingStr[0x20] = "the second field is missing";

const char _sceMpegErrorFmt[0x10] = "[MPEG ERROR]%s\n";

const char _sceMpegVerticalSizeStr[0x18] = "vertical size > 2800";

const char _sceMpegChromaFormatStr[0x28] = "_chroma_format needs to be 1: 420";

const char _sceMpegProfileLevelStr[0x20] = "Unsupported profile/level";

const char _sceMpegSeqScalableExtStr[0x30] = "_sequenceScalableExtension() is not implemented";

const char _sceMpegUnknownExtensionStr[0x18] = "Unknown Extension";

const char _sceMpegPicSpatialScalableExtStr[0x38] = "_pictureSpatialScalableExtension is not supported";

const char _sceMpegPicTemporalScalableExtStr[0x38] = "_pictureTemporalScalableExtension is not supported";

const char _sceMpegInvalidMotionTypeFmt[0x28] = "Invalid modion type -- ignored(%d)";

const char _sceMpegInvalidMotionTypeAFmt[0x20] = "(a) invalid motion_type(%d)-0";

const char _sceMpegInvalidMotionTypeBFmt[0x20] = "(b) invalid motion_type(%d)-1";

const char _sceMpegInvalidMotionTypeCFmt[0x20] = "(c) invalid motion_type(%d)-2";

const char _sceMpegIntraSkipMbStr[0x18] = "intra && skip MB";

const char _sceMpegBdecErrorStr[0x20] = "Error code detected(BDEC)";

const char _sceMpegMbAddrIncFmt[0x38] = "Invalid macroblock_address_increment code(0x%08x)";

const char _sceMpegSkipPictureStr[0x20] = "= Skip to the next picture =";

const char _sceMpegSliceStartCodeFmt[0x28] = "slice_start_code(0x%08x) out of range";

const char _sceMpegSliceA0ErrorStr[0x20] = "_sliceA0(): error happens";

const char _sceMpegTooManyMbStr[0x20] = "Too many macroblocks in picture";

const char _sceMpegSkippedMbIPicStr[0x30] = "skiped macroblock in I picure is not allowed";
