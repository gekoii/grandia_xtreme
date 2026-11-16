#include "common.h"
#include "protos/main.h"


s32 McChecksum(u8 *arg0, u32 arg1) {
    return McAdler32(1, arg0, arg1);
}
