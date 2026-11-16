#include "common.h"
#include "main/n07.h"

void TxtFontClutInit(void)
{
  int new_var;
  txtFontAlphaClut[0][0] = 0x00FFFFFF;
  txtFontAlphaClut[0][1] = 0x08FFFFFF;
  txtFontAlphaClut[0][2] = 0x10FFFFFF;
  txtFontAlphaClut[0][3] = 0x18FFFFFF;
  txtFontAlphaClut[0][4] = 0x20FFFFFF;
  txtFontAlphaClut[0][5] = 0x2DFFFFFF;
  txtFontAlphaClut[0][6] = 0x38FFFFFF;
  txtFontAlphaClut[0][7] = 0x40FFFFFF;
  txtFontAlphaClut[0][8] = 0x48FFFFFF;
  txtFontAlphaClut[0][9] = 0x50FFFFFF;
  txtFontAlphaClut[0][10] = 0x58FFFFFF;
  txtFontAlphaClut[0][11] = 0x60FFFFFF;
  txtFontAlphaClut[0][12] = 0x68FFFFFF;
  txtFontAlphaClut[0][13] = 0x70FFFFFF;
  txtFontAlphaClut[0][14] = 0x78FFFFFF;
  txtFontAlphaClut[0][15] = 0x80FFFFFF;
  txtFontAlphaClut[1][0] = 0x00FFFFFF;
  txtFontAlphaClut[1][1] = 0x0AFFFFFF;
  txtFontAlphaClut[1][2] = 0x17FFFFFF;
  txtFontAlphaClut[1][3] = 0x20FFFFFF;
  txtFontAlphaClut[1][4] = 0x2DFFFFFF;
  txtFontAlphaClut[1][5] = 0x3CFFFFFF;
  txtFontAlphaClut[1][6] = 0x41FFFFFF;
  txtFontAlphaClut[1][7] = 0x4CFFFFFF;
  txtFontAlphaClut[1][8] = 0x55FFFFFF;
  txtFontAlphaClut[1][9] = 0x60FFFFFF;
  txtFontAlphaClut[1][10] = 0x66FFFFFF;
  txtFontAlphaClut[1][11] = 0x6CFFFFFF;
  txtFontAlphaClut[1][12] = 0x74FFFFFF;
  txtFontAlphaClut[1][13] = 0x78FFFFFF;
  txtFontAlphaClut[1][14] = 0x7CFFFFFF;
  txtFontAlphaClut[1][15] = 0x80FFFFFF;
  new_var = 1;
  txtFontImageEnable = new_var;
}
