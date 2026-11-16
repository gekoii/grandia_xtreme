/* A table of pointers inside a block of data, found by a byte offset: a
 * message bank's string table ((u16 **)((u8 *)msg + msg->strOfs)), read
 * through a local T ** and indexed straight from the cast. Its entries
 * are 32-bit EE addresses, as the game's data has them.
 * placed:
 * expect: 22 33 | 33 22
 */
#include <stdio.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { int unk0, unk4, lenOfs, strOfs; } Hdr;
u16 s0[2] = { 0x22, 0 }, s1[2] = { 0x33, 0 };
struct { Hdr h; unsigned int tab[2]; } bank = { { 0, 0, 0, 16 }, { 0, 0 } };
int main(void) {
    Hdr *msg = &bank.h;
    u16 **strs;
    u16 *str;
    bank.tab[0] = (unsigned int)(unsigned long long)s0;
    bank.tab[1] = (unsigned int)(unsigned long long)s1;
    strs = (u16 **)((u8 *)msg + msg->strOfs);
    str = strs[0];
    printf("%x %x\n", str[0], strs[1][0]);
    printf("%x %x\n", ((u16 **)((char *)msg + msg->strOfs))[1][0], (*(u16 **)((u8 *)msg + msg->strOfs))[0]);
    return 0;
}
