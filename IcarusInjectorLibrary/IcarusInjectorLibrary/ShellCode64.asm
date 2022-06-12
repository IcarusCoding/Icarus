PUBLIC ShellCode64
EXTERN puts:PROC

.code

pLocalImageBase$ = 32
pAddressTable$1 = 40
pImportDescriptor$2 = 48
pThreadLocalCallback$3 = 56
pOptHeader$ = 64
hMod$4 = 72
szName$5 = 80
pCtx$ = 112

ShellCode64 PROC
$LN14:
        mov     QWORD PTR [rsp+8], rcx
        sub     rsp, 104
        cmp     QWORD PTR pCtx$[rsp], 0
        jne     SHORT $LN8@ShellCode
        jmp     $LN1@ShellCode
$LN8@ShellCode:
        mov     rax, QWORD PTR pCtx$[rsp]
        mov     rax, QWORD PTR [rax+24]
        mov     QWORD PTR pLocalImageBase$[rsp], rax
        mov     rax, QWORD PTR pLocalImageBase$[rsp]
        movsxd  rax, DWORD PTR [rax+60]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        lea     rax, QWORD PTR [rcx+rax+24]
        mov     QWORD PTR pOptHeader$[rsp], rax
        mov     eax, 8
        imul    rax, rax, 1
        mov     rcx, QWORD PTR pOptHeader$[rsp]
        cmp     DWORD PTR [rcx+rax+116], 0
        je      $LN9@ShellCode
        mov     eax, 8
        imul    rax, rax, 1
        mov     rcx, QWORD PTR pOptHeader$[rsp]
        mov     eax, DWORD PTR [rcx+rax+112]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        add     rcx, rax
        mov     rax, rcx
        mov     QWORD PTR pImportDescriptor$2[rsp], rax
$LN2@ShellCode:
        mov     rax, QWORD PTR pImportDescriptor$2[rsp]
        cmp     DWORD PTR [rax+12], 0
        je      $LN3@ShellCode
        mov     rax, QWORD PTR pImportDescriptor$2[rsp]
        mov     eax, DWORD PTR [rax+12]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        add     rcx, rax
        mov     rax, rcx
        mov     QWORD PTR szName$5[rsp], rax
        mov     rcx, QWORD PTR szName$5[rsp]
        mov     rax, QWORD PTR pCtx$[rsp]
        call    QWORD PTR [rax]
        mov     QWORD PTR hMod$4[rsp], rax
        mov     rax, QWORD PTR pImportDescriptor$2[rsp]
        mov     eax, DWORD PTR [rax+16]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        add     rcx, rax
        mov     rax, rcx
        mov     QWORD PTR pAddressTable$1[rsp], rax
$LN4@ShellCode:
        mov     rax, QWORD PTR pAddressTable$1[rsp]
        cmp     QWORD PTR [rax], 0
        je      SHORT $LN5@ShellCode
        mov     rax, QWORD PTR pAddressTable$1[rsp]
        mov     rcx, -9223372036854775808
        mov     rax, QWORD PTR [rax]
        and     rax, rcx
        test    rax, rax
        je      SHORT $LN10@ShellCode
        mov     rax, QWORD PTR pAddressTable$1[rsp]
        mov     rax, QWORD PTR [rax]
        and     rax, 2147483647
        mov     rdx, rax
        mov     rcx, QWORD PTR hMod$4[rsp]
        mov     rax, QWORD PTR pCtx$[rsp]
        call    QWORD PTR [rax+8]
        mov     rcx, QWORD PTR pAddressTable$1[rsp]
        mov     QWORD PTR [rcx], rax
        jmp     SHORT $LN11@ShellCode
$LN10@ShellCode:
        mov     rax, QWORD PTR pAddressTable$1[rsp]
        mov     rax, QWORD PTR [rax]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        lea     rax, QWORD PTR [rcx+rax+2]
        mov     rdx, rax
        mov     rcx, QWORD PTR hMod$4[rsp]
        mov     rax, QWORD PTR pCtx$[rsp]
        call    QWORD PTR [rax+8]
        mov     rcx, QWORD PTR pAddressTable$1[rsp]
        mov     QWORD PTR [rcx], rax
$LN11@ShellCode:
        mov     rax, QWORD PTR pAddressTable$1[rsp]
        add     rax, 8
        mov     QWORD PTR pAddressTable$1[rsp], rax
        jmp     $LN4@ShellCode
$LN5@ShellCode:
        mov     rax, QWORD PTR pImportDescriptor$2[rsp]
        add     rax, 20
        mov     QWORD PTR pImportDescriptor$2[rsp], rax
        jmp     $LN2@ShellCode
$LN3@ShellCode:
$LN9@ShellCode:
        mov     eax, 8
        imul    rax, rax, 9
        mov     rcx, QWORD PTR pOptHeader$[rsp]
        cmp     DWORD PTR [rcx+rax+116], 0
        je      SHORT $LN12@ShellCode
        mov     eax, 8
        imul    rax, rax, 9
        mov     rcx, QWORD PTR pOptHeader$[rsp]
        mov     eax, DWORD PTR [rcx+rax+112]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        mov     rax, QWORD PTR [rcx+rax+24]
        mov     QWORD PTR pThreadLocalCallback$3[rsp], rax
$LN6@ShellCode:
        cmp     QWORD PTR pThreadLocalCallback$3[rsp], 0
        je      SHORT $LN7@ShellCode
        mov     rax, QWORD PTR pThreadLocalCallback$3[rsp]
        cmp     QWORD PTR [rax], 0
        je      SHORT $LN7@ShellCode
        xor     r8d, r8d
        mov     edx, 1
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        mov     rax, QWORD PTR pThreadLocalCallback$3[rsp]
        call    QWORD PTR [rax]
        mov     rax, QWORD PTR pThreadLocalCallback$3[rsp]
        add     rax, 8
        mov     QWORD PTR pThreadLocalCallback$3[rsp], rax
        jmp     SHORT $LN6@ShellCode
$LN7@ShellCode:
$LN12@ShellCode:
        mov     rax, QWORD PTR pOptHeader$[rsp]
        mov     eax, DWORD PTR [rax+16]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        add     rcx, rax
        mov     rax, rcx
        xor     r8d, r8d
        mov     edx, 1
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        call    rax
        mov     rax, QWORD PTR pCtx$[rsp]
        mov     rcx, QWORD PTR pLocalImageBase$[rsp]
        mov     QWORD PTR [rax+16], rcx
$LN1@ShellCode:
        add     rsp, 104
        ret     0
ShellCode64 ENDP
END
