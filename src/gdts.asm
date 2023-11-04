[global load_gdt]

gdtr:
    dw 0 ; Limit
    dd 0 ; Base

load_gdt:
    mov ax, [esp + 4]   ; ax now contains the limit
    mov [gdtr], ax      ; gdtr first dword now has the limit
    mov eax, [esp + 8]  ; eax now contains the base
    mov [gdtr + 2], eax ; gdtr double-dword now has the base
    lgdt [gdtr]
ret
