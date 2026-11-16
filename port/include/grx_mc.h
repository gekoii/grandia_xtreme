/*
 * Memory cards over host directories (port/src/mc.c).
 *
 * Sony's libmc (src/main 46FE20-4714C0) is replaced at the sceMc* level
 * (port/tools/fixups/mc.py): there is no IOP, so mcserv/mcman's card is a
 * directory holding the PS2 directory tree. Calls keep libmc's protocol:
 * a call queues its command and returns 0, the next sceMcSync runs it and
 * reports the libmc command number and mcman's result code.
 *
 * Two cards, port 0 (slot 1, MEMORY CARD slot 1) and port 1 (slot 2):
 * <base>/card1/ and <base>/card2/, with each card's directory-entry
 * attributes and timestamps in <base>/card1.meta, card2.meta (what a host
 * directory can't hold: the closed flag, the PS2 timestamps, entry order).
 * <base> is SDL_GetPrefPath("grandia_extreme", "memcard"), or GRX_MC_DIR.
 * Files put in a card directory by hand show up with default attributes
 * (a closed file, 0x8497; a directory, 0x8427) and the host's timestamps.
 *
 * What is in each slot: GRX_MC1 and GRX_MC2 = "formatted" (a card,
 * created empty when its directory doesn't exist), "unformatted" (a card
 * the game must format first; formatting it empties its directory) or
 * "absent". Default: card 1 formatted, card 2 absent. grx_mc_set_card
 * changes it while the game runs, as pulling or inserting a card does.
 *
 * Only for the port's own sources (port/src/, port/tests/).
 */
#ifndef GRX_MC_H
#define GRX_MC_H

#include <stdbool.h>

/* What a slot holds */
enum {
    GRX_MC_ABSENT = 0,
    GRX_MC_FORMATTED = 1,
    GRX_MC_UNFORMATTED = 2,
};

/* Reads GRX_MC_DIR, GRX_MC1, GRX_MC2 and sets the cards up. sceMcInit
   calls it; calling it again does nothing. */
void grx_mc_init(void);

/* Points both cards at <dir>/card1, <dir>/card2 (NULL: back to GRX_MC_DIR
   or the pref path). Open files are closed and the cards count as newly
   inserted. false if the directory can't be created. */
bool grx_mc_set_root(const char *dir);

/* Puts a card in port 0 or 1 (GRX_MC_FORMATTED / GRX_MC_UNFORMATTED) or
   takes it out (GRX_MC_ABSENT). Any call is a card change: open files on
   that port are lost, and the next sceMcGetInfo reports a new card
   (-1 formatted, -2 unformatted) or no card. */
void grx_mc_set_card(int port, int state);
int grx_mc_card_state(int port);

/* The card's host directory, with a trailing '/' */
const char *grx_mc_card_dir(int port);

/* Free space as sceMcGetInfo reports it: 1 KB clusters, 8000 on an empty
   8 MB card; 0 without a formatted card */
int grx_mc_free_clusters(int port);

/* Closes the host files still open (at exit) */
void grx_mc_shutdown(void);

#endif
