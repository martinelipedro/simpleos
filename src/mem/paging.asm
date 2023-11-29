[global load_pdbr]
[global get_pdbr]
[global flush_tlb]
[global enable_paging]

load_pdbr:
    mov eax, [esp + 4]
    mov cr3, eax
ret

get_pdbr:
    mov eax, cr3
ret

flush_tlb:
    push eax
    mov eax, [esp + 4]
    cli
    invlpg [eax] 
    sti
    pop eax
ret

enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
ret

[global load_dir]
load_dir:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
ret

[global enablep]
enablep:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
ret