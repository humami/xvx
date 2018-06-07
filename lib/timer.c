#include "timer.h"
#include "types.h"
#include "common.h"
#include "idt.h"
#include "thread.h"

static uint32_t ticks;

static void intr_timer_handler() {
	task_struct *cur_thread = running_thread();

	cur_thread->elapsed_ticks++;
	ticks++;

	if(cur_thread->ticks == 0)
		schedule();
	else
		cur_thread->ticks--;
}

void init_timer() 
{
	outb(0x43, 0x34);

	uint32_t counter_value = 1193180 / FREQUENCY;
    
    uint8_t low = (uint8_t)(counter_value & 0xFF);
    uint8_t high = (uint8_t)((counter_value >> 8) & 0xFF);
	
	outb(0x40, low);
	outb(0x40, high);

	register_interrupt_handler(0x20, intr_timer_handler); 
}
