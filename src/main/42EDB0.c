#include "common.h"

void FileLogResult(s32 *arg0, s32 arg1, s32 arg2) {
    arg0[arg1 % 16] = arg2;
}
