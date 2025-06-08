[global gdt_flush]
[global idt_flush]
[extern gdt_pointer]
[extern idt_pointer]

gdt_flush:
    mov eax, [esp + 4]

    lgdt [eax]
    mov ax, 0x10
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    jmp 0x08:flush2
 
flush2:
    ret

idt_flush:
    lidt [idt_pointer]
    sti
ret