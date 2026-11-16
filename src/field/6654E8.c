#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n27.h"

extern s8 evtMsgTileClassTable[];
extern s8 evtMsgTilePieceTable[];
extern s8 evtMsgTileMergeTable[][33];
extern s8 evtMsgTileJunctionTable[];

void EvtMsgTilePut(s32 x, s32 y, u16 type) {
    u16 c, s0, up, down, left, right, m;
    u16 w;
    s32 hi;

    hi = ((EvtScnrGetCur()->id & 0xF000) == 0xE000) ? 0x40 : 0;
    c = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x];
    if (c >= 0x21) c = 0x21;
    c = evtMsgTileClassTable[c];
    if (c == 0) {
        s0 = type;
    } else {
        up = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - EVT_MSG_TILE_COLS];
        down = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x + EVT_MSG_TILE_COLS];
        left = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - 1];
        right = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x + 1];
        s0 = evtMsgTileMergeTable[type][c];
        if (up >= 0x21) up = 0x21;
        if (down >= 0x21) down = 0x21;
        if (left >= 0x21) left = 0x21;
        if (right >= 0x21) right = 0x21;
        up = evtMsgTileClassTable[up];
        down = evtMsgTileClassTable[down];
        left = evtMsgTileClassTable[left];
        if (s0 == 0xFFFF) s0 = type;
        if (c == 0x21) s0 = type;
        right = evtMsgTileClassTable[right];
        if (s0 == 0x13) {
            m = up == 0x11;
            if (down == 0x11) m |= 2;
            if (left == 0x12) m |= 4;
            if (right == 0x12) m |= 8;
            s0 = evtMsgTileJunctionTable[m];
        }
        if (s0 == 0x12) {
            if (up == 0x11) s0 = 0x10;
            if (s0 == 0x12) {
                if (down == 0x11) s0 = 0xF;
                if (s0 == 0x12) {
                    if (left == 0x11) left = 0xE;
                    if (left == 0x17) left = 0x10;
                    if (left == 0x15) left = 0xF;
                    if (left == 0xD) left = 0x13;
                    if (right == 0x11) right = 0xD;
                    if (right == 0x16) right = 0x10;
                    if (right == 0x14) right = 0xF;
                    if (right == 0xE) right = 0x13;
                }
            }
        }
        if (s0 == 0x11) {
            if (up == 0x12) up = 0xF;
            if (up == 0x17) up = 0xD;
            if (up == 0x16) up = 0xE;
            if (up == 0x10) up = 0x13;
            if (down == 0x12) down = 0x10;
            if (down == 0x15) down = 0xD;
            if (down == 0x14) down = 0xE;
            if (down == 0xF) down = 0x13;
            if (left == 0x12) s0 = 0xE;
            if (s0 == 0x11) {
                if (right == 0x12) s0 = 0xD;
            }
        }
        if (s0 == 0xB) {
            if (up == 0x21) s0 = 0xF;
        }
        if (s0 == 0xC) {
            if (down == 0x21) s0 = 0x10;
        }
        if (s0 == 9) {
            if (left == 0x21) s0 = 0xE;
        }
        if (s0 == 0xA) {
            if (right == 0x21) s0 = 0xD;
        }
        up = evtMsgTilePieceTable[up];
        down = evtMsgTilePieceTable[down];
        left = evtMsgTilePieceTable[left];
        right = evtMsgTilePieceTable[right];
        if (up == 0x21) up = 0x60;
        if (down == 0x21) down = 0x60;
        if (left == 0x21) left = 0x60;
        if (right == 0x21) right = 0x60;
        evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - EVT_MSG_TILE_COLS] = up;
        evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x + EVT_MSG_TILE_COLS] = down;
        evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - 1] = left;
        evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x + 1] = right;
    }
    if (s0 == 0x21) {
        s0 = 0x60;
    } else if (s0 == 0) {
        s0 = 0x60;
    } else {
        s0 = evtMsgTilePieceTable[s0];
    }
    w = EvtScnrGetCur()->id & 0xF000;
    if (w == 0xE000) {
        up = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - EVT_MSG_TILE_COLS];
        left = evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - 1];
        if ((EvtScnrGetCur()->id & 0xF000) == w && s0 != 0 && s0 != 0x60) {
            up -= 0x40;
            left -= 0x40;
        }
        if (s0 == 7) {
            if (up == 1) s0 = 0x11;
        } else if (s0 == 8) {
            if (up == 2) s0 = 0x13;
        } else if (s0 == 5) {
            if (left == 1) s0 = 0x10;
        } else if (s0 == 6) {
            if (left == 3) s0 = 0x14;
        } else if (s0 == 2) {
            if (left == 5) evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - 1] = hi + 0x12;
        } else if (s0 == 3) {
            if (up == 7) evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - EVT_MSG_TILE_COLS] = hi + 0x15;
        } else if (s0 == 4) {
            if (up == 8) evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - EVT_MSG_TILE_COLS] = hi + 0x17;
            if (left == 6) evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x - 1] = hi + 0x16;
        }
        if (s0 != 0 && s0 != 0x60) s0 += hi;
    }
    evtMsgTileGrid[y * EVT_MSG_TILE_COLS + x] = s0;
}
