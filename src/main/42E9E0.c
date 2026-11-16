#include "common.h"
#include "main/n03.h"

s32 FileCmpReadHeader(void *arg0, void *arg1) {
    FileCmpHeader *h = arg0;
    s32 *src = arg1;
    s32 key;
    s32 magic;

    key = src[0];
    h->key = key;
    magic = src[1] ^ key;
    h->magic = magic;
    h->size = src[2] ^ key;
    h->unkC = src[3] ^ key;
    return magic == 0x15246468;
}
