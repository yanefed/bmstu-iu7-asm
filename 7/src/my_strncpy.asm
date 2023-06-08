section .text

global my_strncpy

my_strncpy:
    push ebp
    mov ebp, esp

    mov edi, [ebp + 8]
    mov esi, [ebp + 12]
    mov ecx, [ebp + 16]
    inc ecx

    push ebx
    mov ebx, edi

    cmp edi, esi
    jg copy_from_the_end

    copy_at_first:
        rep movsb
        jmp finish

    copy_from_the_end:
        add edi, ecx
        add esi, ecx
        dec edi
        dec esi

        std
        rep movsb
        cld

    finish:
        mov eax, ebx
        pop ebx
        pop ebp
        ret