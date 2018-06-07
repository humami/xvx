%include "boot.inc"

SECTION LOADER vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR

GDT_BASE: dd 0x00000000
          dd 0x00000000

CODE_DESC: dd 0x0000FFFF
           dd DESC_CODE_HIGH4

DATA_STACK_DESC: dd 0x0000FFFF
                 dd DESC_DATA_HIGH4

VIDEO_DESC: dd 0x80000007
            dd DESC_VIDEO_HIGH4

GDT_SIZE equ $ - GDT_BASE
GDT_LIMIT equ GDT_SIZE - 1
times 60 dq 0

total_mem_bytes dd 0

SELECTOR_CODE equ (0x0001 << 3) + TI_GDT + RPL0
SELECTOR_DATA equ (0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO equ (0x0003 << 3) + TI_GDT + RPL0

gdt_ptr dw GDT_LIMIT
        dd GDT_BASE

ards_buf times 244 db 0
ards_nr dw 0

loader_start:
    [bits 16]
        xor ebx, ebx
        mov edx, 0x534d4150
        mov di, ards_buf
      .e820_men_get_loop:
        mov eax, 0x0000e820
        mov ecx, 20
        int 0x15
        add di, cx
        inc word [ards_nr]
        cmp ebx, 0
        jnz .e820_men_get_loop

        mov cx, [ards_nr]
        mov ebx, ards_buf
        xor edx, edx
      .find_max_mem_area:
        mov eax, [ebx]
        add eax, [ebx + 8]
        add ebx, 20
        cmp edx, eax
        jge .next_ards
        mov edx, eax

      .next_ards:
        loop .find_max_mem_area
        mov [total_mem_bytes], edx

        cli

        in al, 0x92
        or al, 0x02
        out 0x92, al

        lgdt [gdt_ptr]

        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax

        jmp dword SELECTOR_CODE:.p_mode_start

    [bits 32]
      .p_mode_start:
        mov ax, SELECTOR_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov ss, ax
        mov gs, ax
        mov esp, LOADER_STACK_TOP

        ;load kernel
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx

        mov eax, KERNEL_START_SECTOR 
        mov ebx, KERNEL_BASE_ADDR
        mov ecx, 200
        call rd_disk_m_32

        call setup_page

        mov eax, PAGE_DIR_POS
        mov cr3, eax

        mov eax, cr0
        or eax, 0x80000000
        mov cr0, eax

        ;init kernel
        call expand_kernel

        mov esp, 0xc009f000
        jmp eax

    setup_page:
        mov ecx, 4096
        mov esi, 0
      .clear_page_dir:
        mov byte [PAGE_DIR_POS + esi], 0
        inc esi
        loop .clear_page_dir

      .create_pde:
        mov eax, PAGE_DIR_POS
        add eax, 0x1000
        mov ebx, eax

        or eax, PG_US_U | PG_RW_W | PG_P
        mov [PAGE_DIR_POS + 0x0], eax
        mov [PAGE_DIR_POS + 0xc00], eax

        sub eax, 0x1000
        mov [PAGE_DIR_POS + 4092], eax

        mov ecx, 512
        mov esi, 0
        mov edx, PG_US_U | PG_RW_W | PG_P

      .create_pte:
        mov [ebx + esi * 4], edx
        add edx, 4096
        inc esi
        loop .create_pte

        ret

    expand_kernel:
        push ebp
        mov ebp, esp
        sub esp, 4

        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx

        mov eax, KERNEL_BASE_ADDR

        ;entry_address
        mov ebx, [eax + 24]
        mov [ebp - 4], ebx
        
        ;program header table address
        mov ebx, [eax + 28]
        ;program header size
        mov dx, [eax + 42]
        
        ;program header nums
        mov cx, [eax + 44]

        add ebx, KERNEL_BASE_ADDR

      .expand_all_segment:
        push dword [ebx + 16]
        mov eax, dword [ebx + 4]
        add eax, KERNEL_BASE_ADDR
        push eax
        push dword [ebx + 12]
        call mem_cpy
        add esp, 12
        add ebx, edx
        loop .expand_all_segment

        mov eax, [ebp - 4]
        add esp, 4
        pop ebp
        ret

    mem_cpy:
        push ebp
        
        cld
        mov ebp, esp
        push ecx

        mov edi, [ebp + 8]
        mov esi, [ebp + 12]
        mov ecx, [ebp + 16]
        rep movsb

        pop ecx
        pop ebp
        ret

    rd_disk_m_32:
        mov esi, eax
        mov di, cx

        mov dx, 0x1f2
        mov al, cl
        out dx, al

        mov eax, esi

        mov dx, 0x1f3
        out dx, al

        mov cl, 8
        shr eax, cl
        mov dx, 0x1f4
        out dx, al

        shr eax, cl
        mov dx, 0x1f5
        out dx, al

        shr eax, cl
        and al, 0x0f
        or al, 0xe0
        mov dx, 0x1f6
        out dx, al

        mov dx, 0x1f7
        mov al, 0x20
        out dx, al

      .not_ready:
        nop
        in al, dx
        and al, 0x88
        cmp al, 0x08
        jnz .not_ready

        mov ax, di
        mov dx, 256
        mul dx
        mov cx, ax

        mov dx, 0x1f0

      .go_on_read:
        in ax, dx
        mov [ebx], ax
        add ebx, 2
        loop .go_on_read
        ret
