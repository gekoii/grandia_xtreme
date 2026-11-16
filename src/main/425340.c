#include "common.h"
#include "snd.h"
#include "protos/main.h"



extern char dbgFileIopStrIopStat[];
extern char dbgFileIopStrStat[];
extern char dbgFileIopStrSecnum[];
extern char dbgFileIopStrGetofs[];
extern char dbgFileIopStrBufofs[];
extern char dbgFileIopStrBuflen[];
extern char dbgFileIopStrDummy[];
extern char dbgFileIopStrReadlen[];
extern char dbgFileIopStrBufadr[];
extern char dbgFileIopStrReqlen[];
extern char dbgFileIopStrLsn[];


void DbgFileIopStatDraw(void) {
    DbgPrintf(2, 5, dbgFileIopStrIopStat);
    DbgPrintf(2, 6, dbgFileIopStrStat, sndIopStatus->strm[0].stat, sndIopStatus->strm[1].stat, sndIopStatus->strm[2].stat);
    DbgPrintf(2, 7, dbgFileIopStrSecnum, sndIopStatus->strm[0].secNum, sndIopStatus->strm[1].secNum, sndIopStatus->strm[2].secNum);
    DbgPrintf(2, 8, dbgFileIopStrGetofs, sndIopStatus->strm[0].getOfs, sndIopStatus->strm[1].getOfs, sndIopStatus->strm[2].getOfs);
    DbgPrintf(2, 9, dbgFileIopStrBufofs, sndIopStatus->strm[0].bufOfs, sndIopStatus->strm[1].bufOfs, sndIopStatus->strm[2].bufOfs);
    DbgPrintf(2, 10, dbgFileIopStrBuflen, sndIopStatus->strm[0].bufLen, sndIopStatus->strm[1].bufLen, sndIopStatus->strm[2].bufLen);
    DbgPrintf(2, 11, dbgFileIopStrDummy, sndIopStatus->strm[0].dummy, sndIopStatus->strm[1].dummy, sndIopStatus->strm[2].dummy);
    DbgPrintf(2, 12, dbgFileIopStrReadlen, sndIopStatus->strm[0].readLen, sndIopStatus->strm[1].readLen, sndIopStatus->strm[2].readLen);
    DbgPrintf(2, 13, dbgFileIopStrBufadr, sndIopStatus->strm[0].bufAdr, sndIopStatus->strm[1].bufAdr, sndIopStatus->strm[2].bufAdr);
    DbgPrintf(2, 14, dbgFileIopStrReqlen, sndIopStatus->strm[0].reqLen, sndIopStatus->strm[1].reqLen, sndIopStatus->strm[2].reqLen);
    DbgPrintf(2, 15, dbgFileIopStrLsn, sndIopStatus->strm[0].lsn, sndIopStatus->strm[1].lsn, sndIopStatus->strm[2].lsn);
}
