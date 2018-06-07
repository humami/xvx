#include "console.h"
#include "idt.h"
#include "memory.h"
#include "thread.h"


static uint8_t flag = 0;

void k_thread_a(void *arg)
{
    char *para = arg;

    while(1)
    {
        if(flag == 0)
        {
            console_write_color(para, rc_black, rc_red);
            flag = 1;
        }
    }
}

void k_thread_b(void *arg)
{
    char *para = arg;

    while(1)
    {
        if(flag == 1)
        {
            console_write_color(para, rc_black, rc_green);
            flag = 0;
        }
    }
}

int main(void)
{
	console_clear();
	idt_init();
	mem_init();
    thread_init();
    
    console_write("\n");
    console_write_color("Hello, OS kernel!\n", rc_black, rc_red);
    
    /*
    uint32_t men_max_size = (*(uint32_t *)0xb00);
    console_write_color("Mem size: ", rc_black, rc_green);
    console_write_hex(men_max_size, rc_black, rc_red);
    
    void *addr = get_kernel_pages(3);

    console_write("\n");
    uint32_t content = (*(uint32_t *)0x9a000);
    console_write_color("0xc009a000 content: ", rc_black, rc_green);
    console_write_hex(content, rc_black, rc_red);
    */
    intr_enable();

    thread_start("k_thread_a", 1, k_thread_a, "A");
    thread_start("k_thread_b", 1, k_thread_b, "B");
    
    while(1);
    return 0;
} 
