#include "common.h"
#include "main/n03.h"

extern void MemSet(void *, s32, s32);

void FileInit(void) {
    FileReadWork *p = &fileReadWork;
    MemSet(p, 0, sizeof(FileReadWork));
    p->state = -1;
}
