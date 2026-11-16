#include "common.h"
#include "main/n08.h"

s32 McGetCardState(s32 arg0) {
    return mcPortState[arg0].state;
}
