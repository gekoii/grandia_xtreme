#include "common.h"
#include "main/n03.h"

s32 FileReadStart(char *s, s32 arg1) {
    s32 i;

    if (arg1 == 0) {
        return 0;
    }
    if (fileReadWork.state >= 0) {
        return 0;
    }
    i = 0;
    fileReadWork.name[i] = s[i];
    if (s[i] != 0) {
        do {
            i++;
            if (i >= 31) {
                break;
            }
            fileReadWork.name[i] = s[i];
        } while (s[i] != 0);
    }
    fileReadWork.buf = (u8 *)arg1;
    fileReadWork.state = 0;
    fileReadWork.sectors = 0;
    fileReadWork.remain = 0;
    fileReadWork.retry[0] = 0;
    fileReadWork.retry[1] = 0;
    fileReadWork.retry[2] = 0;
    fileReadWork.retry[3] = 0;
    fileReadWork.retry[4] = 0;
    fileReadWork.retry[5] = 0;
    return 1;
}
