#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void FlushCache(s32);
s32 SndStrmGetReady(s32, s32);


s32 FileReadUpdate(s32 arg0) {
    FileFindInfo st;
    s32 r;

    FlushCache(0);
    switch (fileReadWork.state) {
    case -1:
        break;
    case 0:
        r = SndStrmOpen(fileReadWork.name);
        FileLogResult(fileReadWork.log[0], fileReadWork.retry[0], r);
        if (r < 0) {
            fileReadWork.retry[0]++;
            break;
        }
        fileReadWork.fd = r;
        fileReadWork.state++;
        if (SndFsFind((u8 *)fileReadWork.name, (s32 *)&st) > 0) {
            fileReadWork.sectors = st.size >> 11;
            if (st.size & 0x7FF) {
                fileReadWork.sectors++;
            }
            fileReadWork.remain = fileReadWork.sectors;
        } else {
            fileReadWork.state = -1;
        }
        break;
    case 1:
        r = SndStrmReadStart(fileReadWork.fd, fileReadWork.sectors);
        FileLogResult(fileReadWork.log[1], fileReadWork.retry[1], r);
        if (r < 0) {
            fileReadWork.retry[1]++;
            break;
        }
        fileReadWork.state++;
        break;
    case 2:
        r = SndStrmGetReady(fileReadWork.fd, 0);
        FileLogResult(fileReadWork.log[2], fileReadWork.retry[2], r);
        if (r > 0) {
            fileReadWork.chunk = r;
            fileReadWork.state++;
        } else {
            fileReadWork.retry[2]++;
        }
        break;
    case 3:
        r = SndStrmFetchStart(fileReadWork.fd, fileReadWork.buf, fileReadWork.chunk);
        FileLogResult(fileReadWork.log[3], fileReadWork.retry[3], r);
        if (r < 0) {
            fileReadWork.retry[3]++;
            break;
        }
        fileReadWork.state++;
        break;
    case 4:
        r = SndStrmFetchPoll(fileReadWork.fd);
        FileLogResult(fileReadWork.log[4], fileReadWork.retry[4], r);
        if (r == 0) {
            fileReadWork.remain -= fileReadWork.chunk;
            if (fileReadWork.remain <= 0) {
                fileReadWork.state++;
            } else {
                fileReadWork.state = 2;
                fileReadWork.buf += fileReadWork.chunk << 11;
            }
        } else {
            fileReadWork.retry[4]++;
        }
        break;
    case 5:
        r = SndStrmClose(fileReadWork.fd);
        FileLogResult(fileReadWork.log[5], fileReadWork.retry[5], r);
        if (r >= 0) {
            fileReadWork.state = -1;
            fileReadWork.total[0] += fileReadWork.retry[0];
            fileReadWork.total[1] += fileReadWork.retry[1];
            fileReadWork.total[2] += fileReadWork.retry[2];
            fileReadWork.total[3] += fileReadWork.retry[3];
            fileReadWork.total[4] += fileReadWork.retry[4];
            fileReadWork.total[5] += fileReadWork.retry[5];
        } else {
            fileReadWork.retry[5]++;
        }
        break;
    default:
        fileReadWork.state = -1;
        break;
    }
    FlushCache(0);
    return fileReadWork.state;
}
