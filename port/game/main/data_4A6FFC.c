/* Port-only C for data the decompilation still has in assembly (asm/main/data/main/0A6FFC.rodata.s). */
#include "common.h"

const char _sceDmaSyncTimeoutStr[0x18] = "libdma: sync timeout\n";

const char _sceCdCbfuncFmt[0x28] = "sceCdCbfunc= %d sceCdCbfunc_num= %d\n";

const char _sceCdSearchFileBindErrStr[0x20] = "Libcdvd bind err CdSearchFile\n";

const char _sceCdSearchCallFmt[0x18] = "ee call cmd search %s\n";

const char _sceCdSearchNameFmt[0x10] = "search name %s\n";

const char _sceCdSearchSizeFmt[0x10] = "search size %d\n";

const char _sceCdSearchLbnFmt[0x18] = "search loc lbn %d\n";

const char _sceCdNCmdSemaFailFmt[0x28] = "Ncmd fail sema cur_cmd:%d keep_cmd:%d\n";

const char _sceCdNCmdBindErrStr[0x18] = "Libcdvd bind err N CMD\n";

const char _sceCdNCmdWaitStr[0x10] = "N cmd wait\n";

const char _sceCdSCmdWaitStr[0x10] = "S cmd wait\n";

const char _sceCdSCmdSemaFailFmt[0x28] = "Scmd fail sema cur_cmd:%d keep_cmd:%d\n";

const char _sceCdSCmdBindErrStr[0x18] = "Libcdvd bind err S cmd\n";

const char _sceCdInitBindErrFmt[0x20] = "Libcdvd bind err %d CD_Init %d\n";

const char _sceCdExitStr[0x10] = "Libcdvd Exit\n";

const char _sceCdDiskReadyStartStr[0x10] = "DiskReady 0\n";

const char _sceCdDiskReadyBindErrStr[0x20] = "Libcdvd bind err CdDiskReady\n";

const char _sceCdDiskReadyEndStr[0x18] = "DiskReady ended\n";

const char _sceMcBindErrStr[0x18] = "bind error libmc \n";

const char _sceMcOldMcservStr[0x28] = "libmc: too old release of mcserv.irx\n";

const char _sceMcOldMcmanStr[0x28] = "libmc: too old release of mcman.irx\n";

const char _sceMpegPackHeaderFieldFlagStr[0x30] = "pack_header_field_flag needs to be '0' in PS\n";

const char _sceMpegWorkAreaTooSmallStr[0x28] = "The size of work area is too small";

const char _sceMpegWorkAreaSizeStr[0x20] = "work area size is too small";

const char _sceMpegImageAlignFmt[0x48] = "image buffer needs to be aligned to 64byte boundary(0x%08x)";
