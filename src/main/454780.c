#include "common.h"
#include "main/n08.h"

McPortState *McGetPortState(s32 arg0) {
    if (arg0 == 0) {
        return mcPortState;
    }
    return &mcPortState1;
}
