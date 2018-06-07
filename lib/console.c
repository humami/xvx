#include "console.h"
#include "common.h"

static uint16_t *video_memory = (uint16_t *)0xB8000;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_cursor()
{
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;

	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}

static void scroll()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    if(cursor_y >= 25) {
    	int i;

    	for(i = 0; i < 80 * 24; i++)
    		video_memory[i] = video_memory[i + 80];

    	for(i = 24 * 80; i < 80 * 25; i++)
    		video_memory[i] = blank;

    	cursor_y = 24;
    }	
}

void console_clear()
{
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);
    
    int i;
    for(i = 0; i < 80 * 25; i++)
    	video_memory[i] = blank;

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color = (uint8_t)back;
	uint8_t fore_color = (uint8_t)fore;

	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute = (attribute_byte << 8);

    if(c == 0x08 && cursor_x)
    	cursor_x--;
    else if(c == 0x09)
    	cursor_x = (cursor_x + 8) & ~(8-1);
    else if(c == '\r')
    	cursor_x = 0;
    else if(c == '\n') {
    	cursor_x = 0;
    	cursor_y++;
    } else if(c >= ' ') {
    	video_memory[cursor_y * 80 + cursor_x] = c | attribute;
    	cursor_x++;
    }

    if(cursor_x >= 80)
    {
    	cursor_x = 0;
    	cursor_y++;
    }

    scroll();

    move_cursor();
}

void console_write(char *cstr)
{
	while(*cstr)
	    console_putc_color(*cstr++, rc_black, rc_white);
}

void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while(*cstr)
        console_putc_color(*cstr++, back, fore);
}

void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{
    int tmp;
    char c;
    int i;

    console_write_color("0x", back, fore);

    if(n == 0)
    {
        console_putc_color('0', back, fore);
        return;
    }

    for(i = 28; i>=0; i-=4) {
        tmp = (n >> i) & 0xF;

        if(tmp >= 0xA)
        {   
            c = tmp - 0xA + 'a';
            console_putc_color(c, back, fore);
        }
        else
        {
            c = tmp + '0';
            console_putc_color(c, back, fore);
        }
    }
}

void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{   
    char c[32];
    int i = 0;
    int j = 0;

    if(n == 0)
    {
        console_write_color("0", back, fore);
        return;
    }

    while(n % 10 != 0) {
        c[i++] = n % 10 + '0';
        n = n / 10;
    }

    char str[32];

    while(--i)
        str[j++] = c[i];

    str[j++] = c[i];
    str[j] = '\0';

    console_write_color(str, back, fore);
}
