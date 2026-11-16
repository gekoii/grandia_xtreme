#include "common.h"
#include "protos/main.h"


s32 FileRead(char *arg0, s32 arg1) {
    s32 temp_s0;

    if (FileReadStart(arg0, arg1) == 0) {
        return 1;
    }
    do {
        temp_s0 = FileGetReadState();
        SysWaitFrame(0);
    } while (temp_s0 != -1);
    return 0;
}
