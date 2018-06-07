#ifndef IDT_H
#define IDT_H

#include "types.h"

typedef struct idt_entry_t{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  dcount;
    uint8_t  attribute;
    uint16_t offset_high;
}__attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_t{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_ptr_t;

enum intr_status {
    INTR_OFF,
    INTR_ON
};

typedef void(*interrupt_handler_t)(void);

void register_interrupt_handler(uint8_t vec_no, interrupt_handler_t h);

void idt_init();
enum intr_status intr_get_status();
enum intr_status intr_set_status(enum intr_status status);
enum intr_status intr_enable();
enum intr_status intr_disable();
void intr_handler(uint8_t vec_nr);

void intr0();
void intr1();
void intr2();
void intr3();
void intr4();
void intr5();
void intr6();
void intr7();
void intr8();
void intr9();
void intr10();
void intr11();
void intr12();
void intr13();
void intr14();
void intr15();
void intr16();
void intr17();
void intr18();
void intr19();

void intr20();
void intr21();
void intr22();
void intr23();
void intr24();
void intr25();
void intr26();
void intr27();
void intr28();
void intr29();
void intr30();
void intr31();

void intr32();
void intr33();
void intr34();
void intr35();
void intr36();
void intr37();
void intr38();
void intr39();
void intr40();
void intr41();
void intr42();
void intr43();
void intr44();
void intr45();
void intr46();
void intr47();

void intr255();

#endif
