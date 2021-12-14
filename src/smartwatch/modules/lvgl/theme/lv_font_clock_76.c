/*******************************************************************************
 * Size: 76 px
 * Bpp: 2
 * Opts: --font 071MKSDBoldB.TTF --symbols %-/0123456789: -r 0x20 --size 76 --format lvgl --bpp 2 --no-compress -o lv_font_clock_76.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_CLOCK_76
#define LV_FONT_CLOCK_76 1
#endif

#if LV_FONT_CLOCK_76

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0025 "%" */
    0x0, 0x6f, 0xe0, 0x0, 0x0, 0xf, 0xff, 0xfc,
    0x2, 0xff, 0xfe, 0x0, 0x0, 0x2f, 0xff, 0xfc,
    0xb, 0xff, 0xff, 0x80, 0x0, 0x3f, 0xff, 0xf8,
    0x1f, 0xff, 0xff, 0xd0, 0x0, 0x7f, 0xff, 0xf0,
    0x3f, 0xff, 0xff, 0xe0, 0x0, 0xbf, 0xff, 0xf0,
    0x3f, 0xf4, 0xbf, 0xf0, 0x0, 0xff, 0xff, 0xd0,
    0x3f, 0xf0, 0x3f, 0xf0, 0x1, 0xff, 0xff, 0xc0,
    0x7f, 0xf0, 0x3f, 0xf0, 0x3, 0xff, 0xff, 0x80,
    0x7f, 0xf0, 0x3f, 0xf0, 0x3, 0xff, 0xff, 0x40,
    0x7f, 0xf0, 0x3f, 0xf0, 0xb, 0xff, 0xff, 0x0,
    0x7f, 0xf0, 0x3f, 0xf0, 0xf, 0xff, 0xfe, 0x0,
    0x7f, 0xf0, 0x3f, 0xf0, 0x1f, 0xff, 0xfc, 0x0,
    0x7f, 0xf0, 0x3f, 0xf0, 0x2f, 0xff, 0xfc, 0x0,
    0x3f, 0xf0, 0x7f, 0xf0, 0x3f, 0xff, 0xf8, 0x0,
    0x3f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xf0, 0x0,
    0x2f, 0xff, 0xff, 0xd0, 0xff, 0xff, 0xf0, 0x0,
    0xf, 0xff, 0xff, 0x80, 0xff, 0xff, 0xd0, 0x0,
    0x3, 0xff, 0xfe, 0x2, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x7f, 0xe4, 0x3, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x40, 0x0,
    0x0, 0x0, 0x0, 0xb, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x1f, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xf4, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7f, 0xff, 0xf0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x0, 0x2, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xfd, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xf4, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xbf, 0xff, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xff, 0xff, 0xc0, 0xb, 0xfe, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xd0,
    0x0, 0x3, 0xff, 0xff, 0x81, 0xff, 0xff, 0xf0,
    0x0, 0x7, 0xff, 0xff, 0x3, 0xff, 0xff, 0xfc,
    0x0, 0xf, 0xff, 0xff, 0x7, 0xff, 0xff, 0xfd,
    0x0, 0xf, 0xff, 0xfd, 0xb, 0xfe, 0xf, 0xfd,
    0x0, 0x2f, 0xff, 0xfc, 0xf, 0xfd, 0x7, 0xfe,
    0x0, 0x3f, 0xff, 0xf8, 0xf, 0xfc, 0x7, 0xfe,
    0x0, 0x7f, 0xff, 0xf4, 0xf, 0xfc, 0x7, 0xfe,
    0x0, 0xbf, 0xff, 0xf0, 0xf, 0xfc, 0x7, 0xfe,
    0x0, 0xff, 0xff, 0xe0, 0xf, 0xfc, 0x7, 0xfe,
    0x1, 0xff, 0xff, 0xc0, 0xf, 0xfc, 0x7, 0xfe,
    0x2, 0xff, 0xff, 0xc0, 0xf, 0xfd, 0x7, 0xfe,
    0x3, 0xff, 0xff, 0x40, 0xb, 0xfe, 0xf, 0xfd,
    0x7, 0xff, 0xff, 0x0, 0x7, 0xff, 0xff, 0xfc,
    0xf, 0xff, 0xfe, 0x0, 0x3, 0xff, 0xff, 0xfc,
    0xf, 0xff, 0xfd, 0x0, 0x1, 0xff, 0xff, 0xf0,
    0x2f, 0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xc0,
    0x2f, 0xff, 0xf4, 0x0, 0x0, 0xb, 0xfd, 0x0,

    /* U+002D "-" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,

    /* U+002F "/" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xf4,
    0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf4, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xff, 0xf4, 0x0, 0x0,
    0x0, 0x0, 0x2, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7f, 0xff, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xff, 0xfd, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xbf, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x1f,
    0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbf,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x40, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xe0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x2f, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xd0, 0x0, 0x0, 0x0,
    0x0, 0xb, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x1, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0xb, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x1,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfd, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xfd, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xbf, 0xff, 0xf0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0x0,

    /* U+0030 "0" */
    0x0, 0x0, 0x6, 0xff, 0xfe, 0x40, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x7, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x0,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff,
    0xfe, 0x0, 0x2f, 0xff, 0xff, 0x43, 0xff, 0xff,
    0xc0, 0x0, 0x1f, 0xff, 0xfe, 0xf, 0xff, 0xfd,
    0x0, 0x0, 0x2f, 0xff, 0xfc, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0,
    0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0, 0x0,
    0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0, 0x0,
    0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0, 0x0, 0xf,
    0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0, 0x0, 0x3f,
    0xff, 0xf1, 0xff, 0xff, 0x80, 0x0, 0x0, 0xff,
    0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0, 0x3, 0xff,
    0xff, 0x1f, 0xff, 0xf8, 0x0, 0x0, 0xf, 0xff,
    0xfc, 0x7f, 0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff,
    0xf1, 0xff, 0xff, 0x80, 0x0, 0x0, 0xff, 0xff,
    0xc7, 0xff, 0xfe, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x1f, 0xff, 0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc,
    0x7f, 0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1,
    0xff, 0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xc7,
    0xff, 0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f,
    0xff, 0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f,
    0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x3f, 0xff, 0xe0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc3, 0xff, 0xff, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xf, 0xff, 0xff, 0x0,
    0x0, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0x0,
    0x3, 0xff, 0xff, 0xd0, 0x7f, 0xff, 0xff, 0xaa,
    0xbf, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf8, 0x1, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x2, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xe0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x2, 0xff, 0xff, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x5, 0x65, 0x40, 0x0,
    0x0, 0x0,

    /* U+0031 "1" */
    0x7f, 0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0xff,
    0xeb, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xff, 0xff,
    0xfe, 0xbf, 0xff, 0xff, 0xff, 0xaf, 0xff, 0xff,
    0xff, 0xeb, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xff,
    0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x0,
    0xbf, 0xff, 0xe0, 0x0, 0x1f, 0xff, 0xf8, 0x0,
    0x7, 0xff, 0xfe, 0x0, 0x1, 0xff, 0xff, 0x80,
    0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x7, 0xff, 0xfe, 0x0, 0x1, 0xff, 0xff,
    0x80, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x1, 0xff,
    0xff, 0x80, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f,
    0xff, 0xf8, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x1,
    0xff, 0xff, 0x80, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x1f, 0xff, 0xf8, 0x0, 0x7, 0xff, 0xfe, 0x0,
    0x1, 0xff, 0xff, 0x80, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x1f, 0xff, 0xf8, 0x0, 0x7, 0xff, 0xfe,
    0x0, 0x1, 0xff, 0xff, 0x80, 0x0, 0x7f, 0xff,
    0xe0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x7, 0xff,
    0xfe, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0, 0x7f,
    0xff, 0xe0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x7,
    0xff, 0xfe, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0,
    0x7f, 0xff, 0xe0, 0x0, 0x1f, 0xff, 0xf8, 0x0,
    0x7, 0xff, 0xfe, 0x0, 0x1, 0xff, 0xff, 0x80,
    0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x7, 0xff, 0xfe, 0x0, 0x1, 0xff, 0xff,
    0x80, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x1, 0xff,
    0xff, 0x80, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x1f,
    0xff, 0xf8, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x0,
    0xff, 0xff, 0x40,

    /* U+0032 "2" */
    0x0, 0x0, 0x6, 0xff, 0xfe, 0x40, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x7, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff,
    0xfe, 0x0, 0x2f, 0xff, 0xff, 0x43, 0xff, 0xff,
    0xc0, 0x0, 0x1f, 0xff, 0xfe, 0xf, 0xff, 0xfd,
    0x0, 0x0, 0x2f, 0xff, 0xfc, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0,
    0x7, 0xff, 0xfe, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0x2f, 0xff, 0xf8, 0x3f, 0xff, 0xd0, 0x0, 0x0,
    0xff, 0xff, 0xd0, 0x0, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0xd0, 0x0, 0x0, 0x0, 0x0, 0x2f, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xfc, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x0, 0x3f, 0xff, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x1f, 0xff, 0xff, 0x40, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x2f,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff,
    0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xff,
    0xd0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0,

    /* U+0033 "3" */
    0x0, 0x0, 0x1, 0xbf, 0xf9, 0x0, 0x0, 0x0,
    0x0, 0x1, 0xff, 0xff, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x2f, 0xff, 0xff, 0xff, 0xe0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff,
    0xfe, 0x40, 0x7f, 0xff, 0xff, 0x2, 0xff, 0xff,
    0xd0, 0x0, 0x2f, 0xff, 0xfe, 0xf, 0xff, 0xfe,
    0x0, 0x0, 0x2f, 0xff, 0xf8, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xf, 0xff, 0xf4, 0x0, 0x0,
    0xf, 0xff, 0xfc, 0x15, 0x55, 0x40, 0x0, 0x0,
    0x3f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x2f,
    0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xff, 0xf0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff,
    0xd0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xfd, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff,
    0xff, 0x40, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0xf, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xd0,
    0x0, 0x1, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc0,
    0x0, 0xf, 0xff, 0xfe, 0xb, 0xff, 0xff, 0xd0,
    0x1, 0xff, 0xff, 0xf4, 0xf, 0xff, 0xff, 0xf9,
    0xbf, 0xff, 0xff, 0xc0, 0x2f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfd, 0x0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0xf4, 0x0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x1b, 0xff, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x55, 0x40, 0x0,
    0x0, 0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0x0, 0x0, 0x2f, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0xf, 0xff, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x7f, 0xff, 0xf8, 0x0, 0x0,
    0x0, 0x0, 0x2, 0xff, 0xff, 0xe0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x0, 0xbf, 0xff, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0,
    0x0, 0x1f, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,
    0x7, 0xff, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x1,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0xff, 0xf8, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xff, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x7f, 0xff,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff,
    0xff, 0xe0, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x0, 0x0, 0x2f, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xf8, 0x0, 0x0, 0xb, 0xff, 0xff, 0xbf, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf8, 0xff, 0xff,
    0x80, 0x0, 0x1, 0xff, 0xff, 0xd3, 0xff, 0xfe,
    0x0, 0x0, 0xf, 0xff, 0xff, 0xf, 0xff, 0xf8,
    0x0, 0x0, 0x7f, 0xff, 0xf4, 0x3f, 0xff, 0xe0,
    0x0, 0x2, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x80,
    0x0, 0xf, 0xff, 0xfe, 0x3, 0xff, 0xfe, 0x0,
    0x0, 0xbf, 0xff, 0xf0, 0xf, 0xff, 0xf8, 0x0,
    0x3, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xe0, 0x0,
    0x1f, 0xff, 0xfd, 0x0, 0xff, 0xff, 0x80, 0x0,
    0xff, 0xff, 0xf0, 0x3, 0xff, 0xfe, 0x0, 0x7,
    0xff, 0xff, 0x40, 0xf, 0xff, 0xf8, 0x0, 0x3f,
    0xff, 0xfc, 0x0, 0x7f, 0xff, 0xf0, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0xb, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xd0, 0x0,

    /* U+0035 "5" */
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x41,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x41, 0xff, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xfe,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xfe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xff,
    0xa0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x7, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff,
    0xf4, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x15,
    0x55, 0x40, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff,
    0xff, 0x40, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x3f, 0xff, 0xe0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xc0,
    0x0, 0x1, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc0,
    0x0, 0xf, 0xff, 0xfe, 0xb, 0xff, 0xff, 0x80,
    0x0, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 0xea,
    0xaf, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x40, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0x40, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x1, 0x59, 0x50, 0x0,
    0x0, 0x0,

    /* U+0036 "6" */
    0x0, 0x0, 0x6, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0,
    0x7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x40, 0xbf, 0xff,
    0xfd, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfd,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xff, 0xfe, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xff,
    0xa0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x7, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x1f, 0xff, 0xf8, 0x0, 0x2, 0xff, 0xff,
    0xf4, 0x7f, 0xff, 0xe0, 0x0, 0x1, 0xff, 0xff,
    0xe1, 0xff, 0xff, 0x80, 0x0, 0x2, 0xff, 0xff,
    0xc7, 0xff, 0xfe, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x1f, 0xff, 0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc,
    0x7f, 0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1,
    0xff, 0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xc7,
    0xff, 0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f,
    0xff, 0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f,
    0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x3f, 0xff, 0xe0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xc0,
    0x0, 0x1, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xc0,
    0x0, 0xf, 0xff, 0xfe, 0xb, 0xff, 0xff, 0x80,
    0x0, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 0xea,
    0xaf, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x40, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0x40, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x1, 0x59, 0x50, 0x0,
    0x0, 0x0,

    /* U+0037 "7" */
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0, 0x0,
    0x0, 0x0, 0xb, 0xff, 0xfe, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x2f, 0xff, 0xf4, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x1f, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xbf, 0xff, 0xd0, 0x0, 0x0, 0x0, 0x0,
    0x3, 0xff, 0xff, 0x40, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x1,
    0xff, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0xb,
    0xff, 0xfd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xff,
    0xd0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x0, 0x0, 0x0, 0x2, 0xff, 0xff, 0x80,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfd, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x2f, 0xff, 0xf4, 0x0, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0x40, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0,
    0xb, 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x2f, 0xff, 0xf4, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0xbf,
    0xff, 0xd0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff,
    0xff, 0x40, 0x0, 0x0, 0x0, 0x0, 0xf, 0xff,
    0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff,
    0xf0, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0x0, 0x0, 0xb, 0xff, 0xfd,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xfe, 0x0,
    0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x0, 0x6, 0xff, 0xfe, 0x40, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0xb, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x0,
    0xbf, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xbf, 0xff,
    0xfd, 0x0, 0x2f, 0xff, 0xff, 0x43, 0xff, 0xff,
    0xc0, 0x0, 0x1f, 0xff, 0xfe, 0xf, 0xff, 0xfd,
    0x0, 0x0, 0x1f, 0xff, 0xfc, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xf, 0xff, 0xf8, 0x0, 0x0,
    0xf, 0xff, 0xfc, 0x3f, 0xff, 0xf0, 0x0, 0x0,
    0x3f, 0xff, 0xf0, 0xff, 0xff, 0xd0, 0x0, 0x1,
    0xff, 0xff, 0xc2, 0xff, 0xff, 0xc0, 0x0, 0x1f,
    0xff, 0xfd, 0x7, 0xff, 0xff, 0xe0, 0x2, 0xff,
    0xff, 0xf0, 0xf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xe0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x0, 0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
    0x7f, 0xff, 0xfe, 0x0, 0x2f, 0xff, 0xff, 0x2,
    0xff, 0xff, 0xc0, 0x0, 0x1f, 0xff, 0xfd, 0xf,
    0xff, 0xfd, 0x0, 0x0, 0x2f, 0xff, 0xf8, 0x3f,
    0xff, 0xf0, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x3f, 0xff, 0xe0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0xc0,
    0x0, 0x1, 0xff, 0xff, 0xc3, 0xff, 0xff, 0x80,
    0x0, 0xf, 0xff, 0xfe, 0xb, 0xff, 0xff, 0x80,
    0x0, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 0xea,
    0xaf, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x0, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x40, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x80, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0x40, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff, 0xd0,
    0x0, 0x0, 0x0, 0x0, 0x1, 0x59, 0x50, 0x0,
    0x0, 0x0,

    /* U+0039 "9" */
    0x0, 0x0, 0x6, 0xff, 0xfe, 0x40, 0x0, 0x0,
    0x0, 0x2, 0xff, 0xff, 0xff, 0xd0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x0,
    0x7, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x0, 0x0,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0xf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff,
    0xfe, 0x0, 0x2f, 0xff, 0xff, 0x42, 0xff, 0xff,
    0xc0, 0x0, 0x1f, 0xff, 0xfe, 0xf, 0xff, 0xfd,
    0x0, 0x0, 0x2f, 0xff, 0xfc, 0x3f, 0xff, 0xf0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0,
    0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0,
    0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0x80, 0x0,
    0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe, 0x0, 0x0,
    0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xf, 0xff, 0xfc, 0x3f, 0xff, 0xe0, 0x0, 0x0,
    0x3f, 0xff, 0xf0, 0xff, 0xff, 0xc0, 0x0, 0x0,
    0xff, 0xff, 0xc3, 0xff, 0xff, 0x40, 0x0, 0x3,
    0xff, 0xff, 0xf, 0xff, 0xff, 0x40, 0x0, 0xf,
    0xff, 0xfc, 0x1f, 0xff, 0xff, 0x80, 0x0, 0x3f,
    0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x0, 0x0, 0x1b, 0xff, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x15,
    0x55, 0x0, 0x0, 0x0, 0xff, 0xff, 0xc3, 0xff,
    0xfd, 0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff,
    0xe0, 0x0, 0x0, 0x3f, 0xff, 0xf1, 0xff, 0xff,
    0x80, 0x0, 0x0, 0xff, 0xff, 0xc7, 0xff, 0xfe,
    0x0, 0x0, 0x3, 0xff, 0xff, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xf, 0xff, 0xfc, 0x7f, 0xff, 0xe0,
    0x0, 0x0, 0x3f, 0xff, 0xf0, 0xff, 0xff, 0x80,
    0x0, 0x0, 0xff, 0xff, 0xc3, 0xff, 0xff, 0x0,
    0x0, 0x7, 0xff, 0xff, 0xf, 0xff, 0xfe, 0x0,
    0x0, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xfe, 0x0,
    0x3, 0xff, 0xff, 0xd0, 0x7f, 0xff, 0xff, 0xaa,
    0xbf, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf8, 0x1, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0x2, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfd, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xe0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0x0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xfd,
    0x0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0x40,
    0x0, 0x0, 0x0, 0x0, 0x5, 0x65, 0x40, 0x0,
    0x0, 0x0,

    /* U+003A ":" */
    0x3f, 0xff, 0xd7, 0xff, 0xfd, 0x7f, 0xff, 0xd7,
    0xff, 0xfd, 0x7f, 0xff, 0xd7, 0xff, 0xfd, 0x7f,
    0xff, 0xd7, 0xff, 0xfd, 0x3f, 0xff, 0xd0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xff, 0xd7, 0xff, 0xfd, 0x7f, 0xff, 0xd7,
    0xff, 0xfd, 0x7f, 0xff, 0xd7, 0xff, 0xfd, 0x7f,
    0xff, 0xd7, 0xff, 0xfd, 0x3f, 0xff, 0xd0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 207, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 581, .box_w = 32, .box_h = 57, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 456, .adv_w = 557, .box_w = 31, .box_h = 10, .ofs_x = 2, .ofs_y = 24},
    {.bitmap_index = 534, .adv_w = 557, .box_w = 31, .box_h = 57, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 976, .adv_w = 557, .box_w = 31, .box_h = 59, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1434, .adv_w = 557, .box_w = 17, .box_h = 57, .ofs_x = 9, .ofs_y = 0},
    {.bitmap_index = 1677, .adv_w = 557, .box_w = 31, .box_h = 58, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2127, .adv_w = 557, .box_w = 31, .box_h = 58, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 2577, .adv_w = 557, .box_w = 31, .box_h = 57, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 3019, .adv_w = 557, .box_w = 31, .box_h = 58, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 3469, .adv_w = 557, .box_w = 31, .box_h = 58, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 3919, .adv_w = 557, .box_w = 31, .box_h = 57, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 4361, .adv_w = 557, .box_w = 31, .box_h = 58, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 4811, .adv_w = 557, .box_w = 31, .box_h = 59, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 5269, .adv_w = 226, .box_w = 10, .box_h = 25, .ofs_x = 2, .ofs_y = 16}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x5, 0xd
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 14, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 47, .range_length = 12, .glyph_id_start = 4,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t lv_font_clock_76 = {
#else
lv_font_t lv_font_clock_76 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 59,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -10,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_CLOCK_76*/

