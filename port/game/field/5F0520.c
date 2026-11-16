/* Port-only C for ItemCanAdd (0x005F0520), which the decompilation still has in assembly. */
#include "common.h"
#include "party.h"
#include "protos/field.h"
#include "field/n19.h"

extern s16 itemMaxItems;

/* 0 if one more of the item fits in its list, -1 if not. Mana eggs and
   skill books held by a character who isn't in the party don't count
   toward the capacity. */
s32 ItemCanAdd(s16 item) {
    PtyItemSlot *slot;
    s32 ret = -1;
    s32 free = -1;
    s16 n = 0;
    s32 i;
    s32 owner;

    switch (ptyItemTable[item].type) {
    case PTY_ITEM_MANA_EGG:
        if (ptyPartyData->eggs[0].itemId == 0) {
            return 0;
        }
        if (itemEggCapacity - 1 <= 0) {
            return -1;
        }
        for (i = 0;;) {
            owner = ptyPartyData->eggs[i].owner;
            if (owner == -1 || PtyCharIsJoined(owner) >= 0) {
                n++;
            }
            i++;
            if (i >= PTY_EGG_SIZE) {
                return -1;
            }
            if (ptyPartyData->eggs[i].itemId == 0) {
                return 0;
            }
            if (n >= itemEggCapacity - 1) {
                return -1;
            }
        }

    case PTY_ITEM_SKILL_BOOK:
        if (ptyPartyData->books[0].itemId == 0) {
            return 0;
        }
        if (itemBookCapacity - 1 <= 0) {
            return -1;
        }
        for (i = 0;;) {
            owner = ptyPartyData->books[i].owner;
            if (owner == -1 || PtyCharIsJoined(owner) >= 0) {
                n++;
            }
            i++;
            if (i >= PTY_BOOK_SIZE) {
                return -1;
            }
            if (ptyPartyData->books[i].itemId == 0) {
                return 0;
            }
            if (n >= itemBookCapacity - 1) {
                return -1;
            }
        }

    case PTY_ITEM_KEY:
    case 15:
        for (i = 0; i < PTY_KEY_ITEM_SIZE; i++) {
            if (ptyPartyData->keyItems[i].id == 0) {
                return 0;
            }
        }
        return -1;

    case 1:
    case 2:
    case 3:
    case 4:
    case 13:
        /* Stackable: an existing stack below 10 (except type 13), else a
           free slot while the bag has room */
        for (i = 0; i < PTY_BAG_SIZE; i++) {
            slot = &ptyPartyData->bag[i];
            if (slot->id == 0 || slot->count == 0) {
                if (free == -1) {
                    free = i;
                }
                continue;
            }
            n++;
            if (slot->id == item && ptyItemTable[item].type != 13) {
                free = i;
                if (slot->count >= 10) {
                    free = -1;
                    break;
                }
                ret = 0;
            }
        }
        if (ret != -1) {
            return ret;
        }
        if (n < itemMaxItems && free != -1) {
            ret = 0;
        }
        return ret;

    case PTY_ITEM_WEAPON:
    case PTY_ITEM_ARMOR:
    case 7:
    case 8:
    case 9:
    case 10:
        for (i = 0; i < PTY_BAG_SIZE; i++) {
            slot = &ptyPartyData->bag[i];
            if (slot->id == 0 || slot->count == 0) {
                return 0;
            }
            if (n >= itemMaxItems - 1) {
                return -1;
            }
            n++;
        }
        return -1;
    }
    return ret;
}
