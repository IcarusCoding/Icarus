PUBLIC ShellCode86
EXTERN puts:PROC

.686
.model flat,c
.code

_szName$1 = -28
_hMod$2 = -24
_pOptHeader$ = -20
_pThreadLocalCallback$3 = -16
_pImportDescriptor$4 = -12
_pAddressTable$5 = -8
_pLocalImageBase$ = -4
_pCtx$ = 8 

ShellCode86 PROC
        push    ebp
        mov     ebp, esp
        sub     esp, 28
        cmp     DWORD PTR _pCtx$[ebp], 0
        jne     SHORT $LN8@ShellCode
        jmp     $LN1@ShellCode
$LN8@ShellCode:
        mov     eax, DWORD PTR _pCtx$[ebp]
        mov     ecx, DWORD PTR [eax+12]
        mov     DWORD PTR _pLocalImageBase$[ebp], ecx
        mov     edx, DWORD PTR _pLocalImageBase$[ebp]
        mov     eax, DWORD PTR [edx+60]
        mov     ecx, DWORD PTR _pLocalImageBase$[ebp]
        lea     edx, DWORD PTR [ecx+eax+24]
        mov     DWORD PTR _pOptHeader$[ebp], edx
        mov     eax, 8
        shl     eax, 0
        mov     ecx, DWORD PTR _pOptHeader$[ebp]
        cmp     DWORD PTR [ecx+eax+100], 0
        je      $LN9@ShellCode
        mov     edx, 8
        shl     edx, 0
        mov     eax, DWORD PTR _pOptHeader$[ebp]
        mov     ecx, DWORD PTR _pLocalImageBase$[ebp]
        add     ecx, DWORD PTR [eax+edx+96]
        mov     DWORD PTR _pImportDescriptor$4[ebp], ecx
$LN2@ShellCode:
        mov     edx, DWORD PTR _pImportDescriptor$4[ebp]
        cmp     DWORD PTR [edx+12], 0
        je      $LN9@ShellCode
        mov     eax, DWORD PTR _pImportDescriptor$4[ebp]
        mov     ecx, DWORD PTR _pLocalImageBase$[ebp]
        add     ecx, DWORD PTR [eax+12]
        mov     DWORD PTR _szName$1[ebp], ecx
        mov     edx, DWORD PTR _szName$1[ebp]
        push    edx
        mov     eax, DWORD PTR _pCtx$[ebp]
        mov     ecx, DWORD PTR [eax]
        call    ecx
        mov     DWORD PTR _hMod$2[ebp], eax
        mov     edx, DWORD PTR _pImportDescriptor$4[ebp]
        mov     eax, DWORD PTR _pLocalImageBase$[ebp]
        add     eax, DWORD PTR [edx+16]
        mov     DWORD PTR _pAddressTable$5[ebp], eax
$LN4@ShellCode:
        mov     ecx, DWORD PTR _pAddressTable$5[ebp]
        cmp     DWORD PTR [ecx], 0
        je      SHORT $LN5@ShellCode
        mov     edx, DWORD PTR _pAddressTable$5[ebp]
        mov     eax, DWORD PTR [edx]
        and     eax, -2147483648
        je      SHORT $LN10@ShellCode
        mov     ecx, DWORD PTR _pAddressTable$5[ebp]
        mov     edx, DWORD PTR [ecx]
        and     edx, 2147483647
        push    edx
        mov     eax, DWORD PTR _hMod$2[ebp]
        push    eax
        mov     ecx, DWORD PTR _pCtx$[ebp]
        mov     edx, DWORD PTR [ecx+4]
        call    edx
        mov     ecx, DWORD PTR _pAddressTable$5[ebp]
        mov     DWORD PTR [ecx], eax
        jmp     SHORT $LN11@ShellCode
$LN10@ShellCode:
        mov     edx, DWORD PTR _pAddressTable$5[ebp]
        mov     eax, DWORD PTR [edx]
        mov     ecx, DWORD PTR _pLocalImageBase$[ebp]
        lea     edx, DWORD PTR [ecx+eax+2]
        push    edx
        mov     eax, DWORD PTR _hMod$2[ebp]
        push    eax
        mov     ecx, DWORD PTR _pCtx$[ebp]
        mov     edx, DWORD PTR [ecx+4]
        call    edx
        mov     ecx, DWORD PTR _pAddressTable$5[ebp]
        mov     DWORD PTR [ecx], eax
$LN11@ShellCode:
        mov     edx, DWORD PTR _pAddressTable$5[ebp]
        add     edx, 4
        mov     DWORD PTR _pAddressTable$5[ebp], edx
        jmp     SHORT $LN4@ShellCode
$LN5@ShellCode:
        mov     eax, DWORD PTR _pImportDescriptor$4[ebp]
        add     eax, 20
        mov     DWORD PTR _pImportDescriptor$4[ebp], eax
        jmp     $LN2@ShellCode
$LN9@ShellCode:
        mov     ecx, 8
        imul    edx, ecx, 9
        mov     eax, DWORD PTR _pOptHeader$[ebp]
        cmp     DWORD PTR [eax+edx+100], 0
        je      SHORT $LN12@ShellCode
        mov     ecx, 8
        imul    edx, ecx, 9
        mov     eax, DWORD PTR _pOptHeader$[ebp]
        mov     ecx, DWORD PTR [eax+edx+96]
        mov     edx, DWORD PTR _pLocalImageBase$[ebp]
        mov     eax, DWORD PTR [edx+ecx+12]
        mov     DWORD PTR _pThreadLocalCallback$3[ebp], eax
$LN6@ShellCode:
        cmp     DWORD PTR _pThreadLocalCallback$3[ebp], 0
        je      SHORT $LN12@ShellCode
        mov     ecx, DWORD PTR _pThreadLocalCallback$3[ebp]
        cmp     DWORD PTR [ecx], 0
        je      SHORT $LN12@ShellCode
        push    0
        push    1
        mov     edx, DWORD PTR _pLocalImageBase$[ebp]
        push    edx
        mov     eax, DWORD PTR _pThreadLocalCallback$3[ebp]
        mov     ecx, DWORD PTR [eax]
        call    ecx
        mov     edx, DWORD PTR _pThreadLocalCallback$3[ebp]
        add     edx, 4
        mov     DWORD PTR _pThreadLocalCallback$3[ebp], edx
        jmp     SHORT $LN6@ShellCode
$LN12@ShellCode:
        push    0
        push    1
        mov     eax, DWORD PTR _pLocalImageBase$[ebp]
        push    eax
        mov     ecx, DWORD PTR _pOptHeader$[ebp]
        mov     edx, DWORD PTR _pLocalImageBase$[ebp]
        add     edx, DWORD PTR [ecx+16]
        call    edx
        mov     eax, DWORD PTR _pCtx$[ebp]
        mov     ecx, DWORD PTR _pLocalImageBase$[ebp]
        mov     DWORD PTR [eax+8], ecx
$LN1@ShellCode:
        mov     esp, ebp
        pop     ebp
        ret     4
ShellCode86 ENDP
END
