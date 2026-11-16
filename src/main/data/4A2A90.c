/* main executable: the start of a file's .rodata - the texts of the menu
 * help window (the MenuHelpBuild and MenuHelpAdd functions, 419328-41B0A0)
 * and the battle tactics names (MenuHelpBuildTactics 419328). Data-only C
 * (plan step 6b, docs/shifting.md). Don't include the project headers here:
 * declare what the data refers to locally.
 *
 * All the texts are in the menu font's codes: each character is a byte, the
 * ASCII code + 0x1F (a space is 0x9F), followed by 0xF9. They are written
 * as octal escapes with the text in a comment. Several labels the code uses
 * are biased bases of the tables (they index from k > 0) or point between
 * the one-byte strings of a table; they are listed with each table.
 */
/* rodata: 0x004A2A90-0x004A3250 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* Stat labels, menuHelpStatNames[e->kind] (MenuHelpAddStats 41A108).
 * menuHelpElementNames (0x004A2AA3 = +0xB) is a biased base of
 * menuHelpElementTexts below (that - 5 * 9, config/symbol_exprs.ld). */
const char menuHelpStatNames[6][7] = {
    "\140\371\163\371\152\371", /* 0: "ATK" */
    "\143\371\144\371\145\371", /* 1: "DEF" */
    "\154\371\140\371\146\371", /* 2: "MAG" */
    "\150\371\154\371\154\371", /* 3: "IMM" */
    "\140\371\142\371\163\371", /* 4: "ACT" */
    "\154\371\156\371\165\371", /* 5: "MOV" */
};

/* One-byte strings, one per element (1-8), all empty in this build:
 * menuHelpElementLabels (0x004A2AC7) is this - 1 (config/symbol_exprs.ld), indexed by element
 * (MenuHelpAddElement 41A8E8 for k < 5, and with +/- the affinity the
 * GOOD/BAD lists of MenuHelpBuildArmor 419960). */
const char menuHelpElementMarks[8] = { 0 };

/* Element names for k = 5..8 (menuHelpElementNames + k * 9,
 * MenuHelpAddElement; empty in this build), then the texts after them:
 * [4] menuHelpStrElementSuffix (0x004A2AF4), [5] menuHelpStrGood (0x004A2AFD),
 * [6] menuHelpStrBad (0x004A2B06), MenuHelpBuildArmor. */
const char menuHelpElementTexts[7][9] = {
    "", /* 0 */
    "", /* 1 */
    "", /* 2 */
    "", /* 3 */
    "", /* 4 */
    "\146\371\156\371\156\371\143\371", /* 5: "GOOD" */
    "\141\371\140\371\143\371", /* 6: "BAD" */
};

/* Effect headings, menuHelpEffectLabels[n] (MenuHelpAddEffectLabel 41ADD0) */
const char menuHelpEffectLabels[6][31] = {
    "", /* 0 */
    "\162\371\217\371\204\371\202\371\210\371\200\371\213\371\237\371\144\371\205\371\205\371\204\371\202\371\223\371", /* 1: "Special Effect" */
    "\141\371\200\371\223\371\223\371\213\371\204\371\237\371\144\371\205\371\205\371\204\371\202\371\223\371", /* 2: "Battle Effect" */
    "\162\371\217\371\204\371\202\371\210\371\200\371\213\371\237\371\143\371\204\371\205\371\204\371\215\371\222\371\204\371", /* 3: "Special Defense" */
    "\162\371\223\371\200\371\223\371\224\371\222\371\237\371\143\371\204\371\205\371\204\371\215\371\222\371\204\371", /* 4: "Status Defense" */
    "\144\371\215\371\204\371\214\371\230\371\237\371\143\371\204\371\205\371\204\371\215\371\222\371\204\371", /* 5: "Enemy Defense" */
};

/* Target type names in front of "Strong vs." / "Reduces damage from",
 * menuHelpVsTypeNames + type * 7 (MenuHelpAddStrongVs 41AA30,
 * MenuHelpAddDamageReduce 41AB30); empty in this build. menuHelpStatusNames
 * (0x004A2C04 = +0x34) is a biased base of menuHelpStatusTexts below (that
 * - 4 * 17, config/symbol_exprs.ld). */
const char menuHelpVsTypeNames[13][7] = { "" };

/* MenuHelpAddStrongVs: "Strong vs." */
const char menuHelpStrStrongVs[] = "\162\371\223\371\221\371\216\371\215\371\206\371\237\371\225\371\222\371\115\371";

/* Status effect names for bits 4-15 (menuHelpStatusNames[k],
 * MenuHelpAddStatusEffect 41AC30; empty in this build), then [12] the text
 * after them (menuHelpStatusNames[16]) and [13] menuHelpStrImmunity
 * (0x004A2D25, MenuHelpBuildArmor). */
const char menuHelpStatusTexts[14][17] = {
    "", /* 0 */
    "", /* 1 */
    "", /* 2 */
    "", /* 3 */
    "", /* 4 */
    "", /* 5 */
    "", /* 6 */
    "", /* 7 */
    "", /* 8 */
    "", /* 9 */
    "", /* 10 */
    "", /* 11 */
    "\144\371\205\371\205\371\204\371\202\371\223\371", /* 12: "Effect" */
    "\150\371\214\371\214\371\224\371\215\371\210\371\223\371\230\371", /* 13: "Immunity" */
};

/* MenuHelpAddLinkedSkill 41AF50: "Break" */
const char menuHelpStrBreak[] = "\141\371\221\371\204\371\200\371\212\371";

/* MenuHelpAddDamageReduce: "Reduces damage from" */
const char menuHelpStrReducesDamage[] = "\161\371\204\371\203\371\224\371\202\371\204\371\222\371\237\371\203\371\200\371\214\371\200\371\206\371\204\371\237\371\205\371\221\371\216\371\214\371";

/* Target and range of a skill (MenuHelpAddTargetRange 41B0A0): [0] is the
 * heading, then menuHelpTargetNames + target * 9 and menuHelpRangeNames +
 * range * 13 */
const char menuHelpTargetNames[6][9] = {
    "\156\371\141\371\151\371", /* 0: "OBJ" */
    "\140\371\213\371\213\371\230\371", /* 1: "Ally" */
    "\145\371\216\371\204\371", /* 2: "Foe" */
    "\156\371\226\371\215\371", /* 3: "Own" */
    "\140\371\213\371\213\371", /* 4: "All" */
    "\140\371\213\371\213\371\230\371", /* 5: "Ally" */
};

const char menuHelpRangeNames[8][13] = {
    "\161\371\140\371\155\371\146\371\144\371", /* 0: "RANGE" */
    "\162\371\210\371\215\371\206\371\213\371\204\371", /* 1: "Single" */
    "\142\371\210\371\221\371\202\371\213\371\204\371", /* 2: "Circle" */
    "\153\371\210\371\215\371\204\371", /* 3: "Line" */
    "\145\371\200\371\215\371", /* 4: "Fan" */
    "\140\371\213\371\213\371", /* 5: "All" */
    "\140\371\221\371\216\371\224\371\215\371\203\371", /* 6: "Around" */
    "\164\371\215\371\210\371\220\371\224\371\204\371", /* 7: "Unique" */
};

/* Battle tactics, by tactic (MenuHelpBuildTactics: menuTacticsNames +
 * kind * 0x18 and so on): name, title and description */
const char menuTacticsNames[8][24] = {
    "\154\371\200\371\215\371\224\371\200\371\213\371", /* 0: "Manual" */
    "\157\371\213\371\200\371\230\371\237\371\145\371\200\371\210\371\221\371", /* 1: "Play Fair" */
    "\145\371\216\371\202\371\224\371\222\371", /* 2: "Focus" */
    "\146\371\216\371\237\371\166\371\210\371\213\371\203\371", /* 3: "Go Wild" */
    "\154\371\204\371\200\371\215\371\210\371\204\371", /* 4: "Meanie" */
    "\162\371\200\371\205\371\204\371\223\371\230\371\237\371\120\371\222\371\223\371", /* 5: "Safety 1st" */
    "\166\371\210\371\223\371\207\371\203\371\221\371\200\371\226\371", /* 6: "Withdraw" */
    "\146\371\216\371\237\371\162\371\214\371\200\371\221\371\223\371", /* 7: "Go Smart" */
};

const char menuTacticsTitles[8][45] = {
    "\143\371\204\371\205\371\200\371\224\371\213\371\223\371\116\371\155\371\216\371\237\371\163\371\200\371\202\371\223\371\210\371\202\371\222\371", /* 0: "Default/No Tactics" */
    "\144\371\202\371\216\371\215\371\216\371\214\371\210\371\202\371\116\371\155\371\216\371\221\371\214\371\200\371\213\371\237\371\140\371\223\371\223\371\200\371\202\371\212\371", /* 1: "Economic/Normal Attack" */
    "\140\371\206\371\206\371\221\371\204\371\222\371\222\371\210\371\225\371\204\371\116\371\162\371\210\371\215\371\206\371\213\371\204\371\237\371\145\371\216\371\204\371", /* 2: "Aggressive/Single Foe" */
    "\140\371\213\371\213\371\114\371\156\371\224\371\223\371\116\371\140\371\221\371\204\371\200\371\237\371\140\371\223\371\223\371\200\371\202\371\212\371", /* 3: "All-Out/Area Attack" */
    "\162\371\224\371\217\371\217\371\216\371\221\371\223\371\116\371\161\371\204\371\200\371\221\371\237\371\162\371\204\371\221\371\225\371\210\371\202\371\204\371", /* 4: "Support/Rear Service" */
    "\142\371\200\371\224\371\223\371\210\371\216\371\224\371\222\371\116\371\161\371\204\371\222\371\223\371\216\371\221\371\204\371", /* 5: "Cautious/Restore" */
    "\157\371\200\371\222\371\222\371\210\371\225\371\204\371\116\371\143\371\204\371\205\371\204\371\215\371\222\371\204\371", /* 6: "Passive/Defense" */
    "\140\371\213\371\213\371\237\371\140\371\221\371\216\371\224\371\215\371\203\371\116\371\141\371\200\371\213\371\200\371\215\371\202\371\204\371", /* 7: "All Around/Balance" */
};

const char menuTacticsDescs[8][67] = {
    "\150\371\215\371\217\371\224\371\223\371\237\371\216\371\221\371\203\371\204\371\221\371\222\371\237\371\203\371\210\371\221\371\204\371\202\371\223\371\213\371\230\371\115\371", /* 0: "Input orders directly." */
    "\145\371\210\371\206\371\207\371\223\371\237\371\216\371\215\371\213\371\230\371\237\371\226\371\210\371\223\371\207\371\237\371\215\371\216\371\221\371\214\371\200\371\213\371\237\371\214\371\204\371\223\371\207\371\216\371\203\371\222\371\115\371", /* 1: "Fight only with normal methods." */
    "\161\371\204\371\203\371\224\371\202\371\204\371\237\371\216\371\215\371\204\371\237\371\205\371\216\371\204\371\237\371\200\371\223\371\237\371\200\371\237\371\223\371\210\371\214\371\204\371\115\371", /* 2: "Reduce one foe at a time." */
    "\140\371\223\371\223\371\200\371\202\371\212\371\237\371\226\371\210\371\223\371\207\371\237\371\214\371\200\371\206\371\210\371\202\371\237\371\200\371\215\371\203\371\237\371\222\371\212\371\210\371\213\371\213\371\222\371\115\371", /* 3: "Attack with magic and skills." */
    "\146\371\200\371\210\371\215\371\237\371\224\371\217\371\217\371\204\371\221\371\237\371\207\371\200\371\215\371\203\371\237\371\226\371\210\371\223\371\207\371\237\371\222\371\224\371\217\371\217\371\216\371\221\371\223\371\115\371", /* 4: "Gain upper hand with support." */
    "\161\371\204\371\222\371\223\371\216\371\221\371\204\371\237\371\205\371\221\371\204\371\220\371\224\371\204\371\215\371\223\371\213\371\230\371\237\371\223\371\216\371\237\371\222\371\224\371\221\371\225\371\210\371\225\371\204\371\115\371", /* 5: "Restore frequently to survive." */
    "\162\371\224\371\217\371\217\371\216\371\221\371\223\371\237\371\200\371\213\371\213\371\210\371\204\371\222\371\237\371\226\371\210\371\223\371\207\371\216\371\224\371\223\371\237\371\200\371\223\371\223\371\200\371\202\371\212\371\210\371\215\371\206\371\115\371", /* 6: "Support allies without attacking." */
    "\140\371\215\371\200\371\213\371\230\371\231\371\204\371\237\371\223\371\207\371\204\371\237\371\205\371\216\371\204\371\222\371\237\371\200\371\215\371\203\371\237\371\222\371\210\371\223\371\224\371\200\371\223\371\210\371\216\371\215\371\115\371", /* 7: "Analyze the foes and situation." */
};
