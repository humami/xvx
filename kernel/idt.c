#include "idt.h"
#include "string.h"
#include "common.h"
#include "console.h"
#include "timer.h"

#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

static idt_entry_t idt_entries[256];

static idt_ptr_t idt_ptr;

static interrupt_handler_t interrupt_handlers[256];

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t attr);

extern void idt_flush(uint32_t);

void register_interrupt_handler(uint8_t vec_no, interrupt_handler_t h) {
    interrupt_handlers[vec_no] = h;
}

void idt_init()
{
	console_write_color("idt_init start\n", rc_black, rc_green);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xfe);
    outb(0xA1, 0xff);

    bzero(&interrupt_handlers, sizeof(interrupt_handler_t) * 256);
	bzero(&idt_entries, sizeof(idt_entry_t) * 256);
    
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    idt_set_gate(0,  (uint32_t)intr0,   0x08,  0x8E);
    idt_set_gate(1,  (uint32_t)intr1,   0x08,  0x8E);
    idt_set_gate(2,  (uint32_t)intr2,   0x08,  0x8E);
    idt_set_gate(3,  (uint32_t)intr3,   0x08,  0x8E);
    idt_set_gate(4,  (uint32_t)intr4,   0x08,  0x8E);
    idt_set_gate(5,  (uint32_t)intr5,   0x08,  0x8E);
    idt_set_gate(6,  (uint32_t)intr6,   0x08,  0x8E);
    idt_set_gate(7,  (uint32_t)intr7,   0x08,  0x8E);
    idt_set_gate(8,  (uint32_t)intr8,   0x08,  0x8E);
    idt_set_gate(9,  (uint32_t)intr9,   0x08,  0x8E);
    idt_set_gate(10, (uint32_t)intr10,  0x08,  0x8E);
    idt_set_gate(11, (uint32_t)intr11,  0x08,  0x8E);
    idt_set_gate(12, (uint32_t)intr12,  0x08,  0x8E);
    idt_set_gate(13, (uint32_t)intr13,  0x08,  0x8E);
    idt_set_gate(14, (uint32_t)intr14,  0x08,  0x8E);
    idt_set_gate(15, (uint32_t)intr15,  0x08,  0x8E);
    idt_set_gate(16, (uint32_t)intr16,  0x08,  0x8E);
    idt_set_gate(17, (uint32_t)intr17,  0x08,  0x8E);
    idt_set_gate(18, (uint32_t)intr18,  0x08,  0x8E);
    idt_set_gate(19, (uint32_t)intr19,  0x08,  0x8E);

    idt_set_gate(20, (uint32_t)intr20,  0x08,  0x8E);
    idt_set_gate(21, (uint32_t)intr21,  0x08,  0x8E);
    idt_set_gate(22, (uint32_t)intr22,  0x08,  0x8E);
    idt_set_gate(23, (uint32_t)intr23,  0x08,  0x8E);
    idt_set_gate(24, (uint32_t)intr24,  0x08,  0x8E);
    idt_set_gate(25, (uint32_t)intr25,  0x08,  0x8E);
    idt_set_gate(26, (uint32_t)intr26,  0x08,  0x8E);
    idt_set_gate(27, (uint32_t)intr27,  0x08,  0x8E);
    idt_set_gate(28, (uint32_t)intr28,  0x08,  0x8E);
    idt_set_gate(29, (uint32_t)intr29,  0x08,  0x8E);
    idt_set_gate(30, (uint32_t)intr30,  0x08,  0x8E);
    idt_set_gate(31, (uint32_t)intr31,  0x08,  0x8E);

    idt_set_gate(32, (uint32_t)intr32,  0x08,  0x8E);
    idt_set_gate(33, (uint32_t)intr33,  0x08,  0x8E);
    idt_set_gate(34, (uint32_t)intr34,  0x08,  0x8E);
    idt_set_gate(35, (uint32_t)intr35,  0x08,  0x8E);
    idt_set_gate(36, (uint32_t)intr36,  0x08,  0x8E);
    idt_set_gate(37, (uint32_t)intr37,  0x08,  0x8E);
    idt_set_gate(38, (uint32_t)intr38,  0x08,  0x8E);
    idt_set_gate(39, (uint32_t)intr39,  0x08,  0x8E);
    idt_set_gate(40, (uint32_t)intr40,  0x08,  0x8E);
    idt_set_gate(41, (uint32_t)intr41,  0x08,  0x8E);
    idt_set_gate(42, (uint32_t)intr42,  0x08,  0x8E);
    idt_set_gate(43, (uint32_t)intr43,  0x08,  0x8E);
    idt_set_gate(44, (uint32_t)intr44,  0x08,  0x8E);
    idt_set_gate(45, (uint32_t)intr45,  0x08,  0x8E);
    idt_set_gate(46, (uint32_t)intr46,  0x08,  0x8E);
    idt_set_gate(47, (uint32_t)intr47,  0x08,  0x8E);

    idt_set_gate(255, (uint32_t)intr255,  0x08,  0x8E);
    
    idt_flush((uint32_t)&idt_ptr);

    init_timer();
}

enum intr_status intr_get_status()
{
    uint32_t eflags = 0;
    GET_EFLAGS(eflags);

    if(EFLAGS_IF & eflags)
        return INTR_ON;

    return INTR_OFF;
}

enum intr_status intr_set_status(enum intr_status status)
{
    if(status & INTR_ON)
        return intr_enable();

    return intr_disable();
}

enum intr_status intr_enable()
{
    if(INTR_ON == intr_get_status())
        return INTR_ON;
    else
    {
        asm volatile("sti");
        return INTR_OFF;
    }
}

enum intr_status intr_disable()
{
    if(INTR_ON == intr_get_status())
    {
        asm volatile("cli");
        return INTR_ON;
    }
    else
        return INTR_OFF;
}

void intr_handler(uint8_t vec_nr)
{   
	if(vec_nr == 0x27 || vec_nr == 0x2f)
		return;
   
    if(interrupt_handlers[vec_nr])
    {
        interrupt_handlers[vec_nr]();
    }
    else 
    {
        console_write_color("int vector: ", rc_black, rc_red);
        console_write_hex(vec_nr, rc_black, rc_red);
        console_write("\n");
    }
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t attr)
{
	idt_entries[num].offset_low = base & 0xFFFF;
	idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
	idt_entries[num].dcount = 0;
	idt_entries[num].selector = sel;
	idt_entries[num].attribute = attr;
}
