#include "common.h"
#include "protos/main.h"
#include "main/n03.h"

extern u8 dbgFileInfoStrFileName[];
extern u8 dbgFileInfoStrFlag[];
extern u8 dbgFileInfoStrMode[];
extern u8 dbgFileInfoStrHandle[];
extern u8 dbgFileInfoStrAddress[];
extern u8 dbgFileInfoStrReadLen[];
extern u8 dbgFileInfoStrReadSect[];
extern u8 dbgFileInfoStrFileLen[];
extern u8 dbgFileInfoStrFileSect[];
extern u8 dbgFileInfoStrCdbufLen[];
extern u8 dbgFileInfoStrOpsHeader[];
extern u8 dbgFileInfoStrOpsHex[];

void DbgFileInfoDraw(void) {
    DbgPrintf(2, 5, dbgFileInfoStrFileName, fileReadWork.name);
    DbgPrintf(2, 6, dbgFileInfoStrFlag, fileReadWork.unk0);
    DbgPrintf(2, 7, dbgFileInfoStrMode, fileReadWork.state);
    DbgPrintf(2, 8, dbgFileInfoStrHandle, fileReadWork.fd);
    DbgPrintf(2, 9, dbgFileInfoStrAddress, fileReadWork.buf);
    DbgPrintf(2, 10, dbgFileInfoStrReadLen, fileReadWork.remain);
    DbgPrintf(2, 11, dbgFileInfoStrReadSect, fileReadWork.unk14);
    DbgPrintf(2, 12, dbgFileInfoStrFileLen, fileReadWork.sectors);
    DbgPrintf(2, 13, dbgFileInfoStrFileSect, fileReadWork.unk1C);
    DbgPrintf(2, 14, dbgFileInfoStrCdbufLen, fileReadWork.chunk);
    DbgPrintf(2, 16, dbgFileInfoStrOpsHeader);
    DbgPrintf(2, 17, dbgFileInfoStrOpsHex, fileReadWork.total[0], fileReadWork.total[1], fileReadWork.total[2],
                  fileReadWork.total[3], fileReadWork.total[4], fileReadWork.total[5]);
    DbgPrintf(2, 18, dbgFileInfoStrOpsHex, fileReadWork.retry[0], fileReadWork.retry[1], fileReadWork.retry[2],
                  fileReadWork.retry[3], fileReadWork.retry[4], fileReadWork.retry[5]);
}
