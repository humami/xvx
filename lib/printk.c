#include "debug.h"
#include "vargs.h"
#include "console.h"
#include "idt.h"

/*
static int vsprintf(char *buff, const char *format, va_list args);

void printk(const char *format, ...)
{
    static char buff[1024];
    int i;

    va_list args;

    va_start(args, format);
    i = vsprintf(buff, format, args);
    va_end(args);

    buff[i] = '\0';

    console_write(buff);
}

void printk_color(real_color_t back, real_color_t fore, const char *format, ...)
{
    static char buff[1024];
    int i;

    va_list args;

    va_start(args, format);
    i = vsprintf(buff, format, args);
    va_end(args);

    buff[i] = '\0';

    console_write_color(buff, back, fore);
}
*/

void panic_spin(char *filename, int line, const char *func, const char *condition) {
    intr_disable();

    console_write("\n\n\n!!!!!error!!!!!!\n");
    console_write("filename: ");console_write(filename);console_write("\n");
    console_write("line: ");console_write_dec(line, rc_black, rc_white);console_write("\n");
    console_write("funciton: ");console_write((char *)func);console_write("\n");
    console_write("condition: ");console_write((char *)condition);console_write("\n");
    while(1);
}
