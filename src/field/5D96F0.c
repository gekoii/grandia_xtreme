#include "common.h"
#include "pad.h"
#include "save.h"
#include "protos/field.h"


extern s32 dbgFldFlagEditColumn;
extern s32 dbgFldFlagEditTop;
extern s32 dbgFldFlagEditLine;
extern u8 dbgFlagEditStrStxAdr[];
extern u8 dbgFlagEditStrRule[];
extern u8 dbgFlagEditStrRowNo[];
extern u8 dbgFlagEditStrBit[];
extern u8 dbgFlagEditStrBitSel[];
extern u8 dbgFlagEditStrRowAdr[];
extern u8 dbgFlagEditStrRow[];

void DbgFldFlagEditPage(u8 *buf, s32 size) {
    Pad *pad = &padPort1;
    s32 n;
    u16 base;
    s32 idx;
    s32 i;
    s32 y;
    s32 j;

    n = size * 8;
    n /= 16;
    if (pad->repeatFast & 0x1000) {
        if (dbgFldFlagEditTop != 0) {
            if (dbgFldFlagEditLine >= 7) {
                dbgFldFlagEditLine--;
            } else {
                dbgFldFlagEditTop--;
            }
        } else if (dbgFldFlagEditLine != 0) {
            dbgFldFlagEditLine--;
        }
    }
    if (pad->repeatFast & 0x4000) {
        if (dbgFldFlagEditLine < 6) {
            dbgFldFlagEditLine++;
        } else {
            dbgFldFlagEditTop++;
        }
    }
    if (n < dbgFldFlagEditTop) {
        dbgFldFlagEditTop = n;
        if (n >= 8) {
            dbgFldFlagEditTop = n - 7;
        }
    }
    n -= dbgFldFlagEditTop;
    if (n - 1 < dbgFldFlagEditLine) {
        dbgFldFlagEditLine = n - 1;
    }
    if (n > 12) {
        n = 12;
    }
    if (pad->repeatFast & 0x8000) {
        if (dbgFldFlagEditColumn != 0) {
            dbgFldFlagEditColumn--;
        } else {
            dbgFldFlagEditColumn = 15;
        }
    }
    if (pad->repeatFast & 0x2000) {
        if (dbgFldFlagEditColumn < 15) {
            dbgFldFlagEditColumn++;
        } else {
            dbgFldFlagEditColumn = 0;
        }
    }
    base = dbgFldFlagEditTop * 16;
    DbgFldBitCursorDraw(dbgFldFlagEditColumn * 2 + 11, 5);
    DbgDrawCursor(3, dbgFldFlagEditLine + 7);
    idx = base + dbgFldFlagEditLine * 16 + dbgFldFlagEditColumn;
    DbgPrintf(0x1C, 4, dbgFlagEditStrStxAdr, (buf - (u8 *)&evtMainFlag) + idx / 8, idx % 8, idx);
    if (pad->repeatFast & 0x20) {
        if (DbgFldBitGet(buf, idx)) {
            DbgFldBitSet(buf, idx, 0);
        } else {
            DbgFldBitSet(buf, idx, 1);
        }
    }
    if (pad->repeatFast & 0x80) {
        if (pad->held & 1) {
            for (i = 0; i < idx; i++) {
                DbgFldBitSet(buf, i, 1);
            }
        } else {
            for (i = 0; i < idx; i++) {
                DbgFldBitSet(buf, i, 0);
            }
        }
    }
    DbgPrintf(4, 6, dbgFlagEditStrRule);
    for (i = 0; i < n; i++) {
        if (dbgFldFlagEditLine == i) {
            y = i + 7;
            DbgPrintf(4, y, dbgFlagEditStrRowNo, base);
            for (j = 0; j < 16; j++) {
                if (dbgFldFlagEditColumn != j) {
                    DbgPrintf(j * 2 + 11, y, dbgFlagEditStrBit, DbgFldBitGet(buf, base + j));
                } else {
                    DbgPrintf(j * 2 + 11, y, dbgFlagEditStrBitSel, DbgFldBitGet(buf, base + j));
                }
            }
            DbgPrintf(0x2B, y, dbgFlagEditStrRowAdr, (buf - (u8 *)&evtMainFlag) + (base >> 3));
        } else {
            DbgPrintf(4, i + 7, dbgFlagEditStrRow, base, DbgFldBitGet(buf, base), DbgFldBitGet(buf, base + 1),
                          DbgFldBitGet(buf, base + 2), DbgFldBitGet(buf, base + 3), DbgFldBitGet(buf, base + 4),
                          DbgFldBitGet(buf, base + 5), DbgFldBitGet(buf, base + 6), DbgFldBitGet(buf, base + 7),
                          DbgFldBitGet(buf, base + 8), DbgFldBitGet(buf, base + 9), DbgFldBitGet(buf, base + 10),
                          DbgFldBitGet(buf, base + 11), DbgFldBitGet(buf, base + 12), DbgFldBitGet(buf, base + 13),
                          DbgFldBitGet(buf, base + 14), DbgFldBitGet(buf, base + 15), (buf - (u8 *)&evtMainFlag) + (base >> 3));
        }
        base += 16;
    }
    DbgPrintf(4, i + 7, dbgFlagEditStrRule);
}
