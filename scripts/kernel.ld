/*
 *      kernel.ld -- 针对 kernel 格式所写的链接脚本
 */
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
ENTRY(main)
SECTIONS
{
    /* 段起始位置 */

    . = 0xc0100000;
    .text : AT(0x100000)
    {
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
    }

    .stab :
    {
        *(.stab)
        . = ALIGN(4096);
    }
    
    .stabstr :
    {
        *(.stabstr)
        . = ALIGN(4096);
    }

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
