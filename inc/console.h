#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

typedef enum real_color {
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_megenta = 5,
    rc_brown = 6,
    rc_white = 7,
    rc_gray = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_megenta = 13,
    rc_yellow = 14,
    rc_light_white = 15 
}real_color_t;

void console_init();

void console_acquire();

void console_release();

void sync_write(char *cstr);

void sync_write_color(char *cstr, real_color_t back, real_color_t fore);

void sync_write_hex(uint32_t n, real_color_t back, real_color_t fore);

void sync_write_dec(uint32_t n, real_color_t back, real_color_t fore);

void console_putc_color(char c, real_color_t back, real_color_t fore);

void console_write(char *cstr);

void console_write_color(char *cstr, real_color_t back, real_color_t fore);

void console_write_hex(uint32_t n, real_color_t back, real_color_t fore);

void console_write_dec(uint32_t n, real_color_t back, real_color_t fore);

#endif
