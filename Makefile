OBJ_DIR = ./obj

CC = gcc
LD = ld
AS = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I inc
L_FLAGS = -m elf_i386 -T scripts/kernel.ld
AS_FLAGS = -f elf

OBJS = $(OBJ_DIR)/kernel.o $(OBJ_DIR)/console.o $(OBJ_DIR)/common.o $(OBJ_DIR)/string.o $(OBJ_DIR)/idt.o $(OBJ_DIR)/idts.o \
$(OBJ_DIR)/timer.o $(OBJ_DIR)/bitmap.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/thread.o $(OBJ_DIR)/list.o $(OBJ_DIR)/switch.o 

all: $(OBJS) link update_img

$(OBJ_DIR)/kernel.o:kernel/kernel.c inc/console.h inc/idt.h inc/memory.h inc/thread.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/console.o:lib/console.c inc/console.h inc/common.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/common.o:lib/common.c inc/common.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/string.o:lib/string.c inc/string.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/timer.o:lib/timer.c inc/timer.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/bitmap.o:lib/bitmap.c inc/bitmap.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/list.o:lib/list.c inc/list.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/idt.o:kernel/idt.c inc/idt.h
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/idts.o:kernel/idt_s.s
	$(AS) $(AS_FLAGS) $< -o $@

$(OBJ_DIR)/switch.o:kernel/switch.s
	$(AS) $(AS_FLAGS) $< -o $@

$(OBJ_DIR)/memory.o:kernel/memory.c inc/memory.h 
	$(CC) $(C_FLAGS) $< -o $@

$(OBJ_DIR)/thread.o:kernel/thread.c inc/thread.h
	$(CC) $(C_FLAGS) $< -o $@

link:
	$(LD) $(L_FLAGS) $(OBJS) -o $(OBJ_DIR)/kernel

.PHONY:update_img
update_img:
	dd if=obj/kernel of=xvx.img bs=512 count=200 seek=9 conv=notrunc

.PHONY:clean
clean:
	cd obj && rm -f ./*
