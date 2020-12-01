%include "io.inc"
section .data
_msgQtdChar    dd  'Foram lidos '
_MSGQTDCHARSIZE EQU  $-_msgQtdChar
_msgQtdChar2    dd  ' caracter(es)'
_MSGQTDCHAR2SIZE EQU  $-_msgQtdChar2
_nwln   dd	0dh,0ah
_NWLNSIZE	EQU	$-_nwln
_NUMBERSIZE	EQU	4
_CHARSIZE	EQU	1
_STRINGSIZE	EQU	100	


section .bss
_number resd    1
_numberStr  resd   1
_char  resd   1
_string  resd   25
_qtdChars   resd    1
_flagSigned resd    1

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    
    ;push eax
    ;call LeerInteiro
    ;call _showQtdRead
    ;pop eax
    
    ;push _number
    ;call EscreverInteiro
    
    push eax
    call LeerChar
    call _showQtdRead
    pop eax
    
    push _char
    call EscreverChar
    
    ;push eax
    ;call LeerString
    ;call _showQtdRead
    ;pop eax
    
    ;push _string
    ;call EscreverString
    
    
    mov eax,1
    mov ebx,0
    int 80h

LeerInteiro:
    enter 0,0
    push ebx
    push ecx
    push edx
    push esi
    
    mov eax,3
    mov ebx,0
    mov ecx,_numberStr
    mov edx,_NUMBERSIZE
    int 80h
    push eax
    mov ecx,eax
    ;cmp ecx,1   ;ecx igual a 1 indica que há apenas [enter] como input
    ;je  _fimStringInteiro
    sub eax,eax
    sub ebx,ebx
    mov esi,0
    cmp byte [_numberStr],'-'
    jne _converterStringInteiro
    inc esi
    mov dword [_flagSigned],1
_converterStringInteiro:
    mov bl,[_numberStr+esi]
    sub ebx,0x30
    add eax,ebx
    inc esi
    cmp byte [_numberStr+esi],0ah
    je  _verificaNegativo
    cmp esi,ecx
    je  _verificaNegativo
    mov ebx,10
    imul ebx
    jmp _converterStringInteiro
_verificaNegativo:
    cmp dword [_flagSigned],0
    je  _armazenaInteiro
    neg eax ;o número contido em eax é convertido para negativo
    mov dword [_flagSigned],0
_armazenaInteiro:
    mov dword [_number],eax
    
    pop eax
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret

    
EscreverInteiro:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    push edi
    push esi
    
    mov ebx,[ebp+8]
    mov eax,[ebx]
    mov esi,0
    test eax,eax
    jns _converterInteiroString
    mov dword [_flagSigned],1
    neg eax
    mov dword [ebx+esi],'-'
    inc esi
_converterInteiroString:
    mov ecx,10
    cdq
    idiv ecx
    add edx,0x30    ;converte o resto binário em edx para ASCII
    ;mov dword [ebx+esi], edx
    push edx
    cmp eax,0   ;compara o quociente binário em eax é igual a zero
    jne _notZero
    jmp _incremento
_notZero:
    mov ecx,esi
    inc ecx ; ecx vale esi+1
    mov edi,[ebx+esi]
    mov dword [ebx+ecx],edi
_incremento:
    inc esi
    cmp eax,0
    jne _converterInteiroString
    mov ecx,esi
    mov esi,0
    cmp dword [_flagSigned],0
    je  _recuperaDigitos
    inc esi
    mov dword [_flagSigned],0
_recuperaDigitos:
    pop dword [ebx+esi]
    inc esi
    cmp esi,ecx
    jne _recuperaDigitos
    mov edx,_NUMBERSIZE
    mov ecx,ebx
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_NWLNSIZE
    mov ecx,_nwln
    mov ebx,1
    mov eax,4
    int 80h
    
    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret 4


LeerChar:
    enter 0,0
    push ebx
    push ecx
    push edx
    
    mov eax,3
    mov ebx,0
    mov ecx,_char
    mov edx,_CHARSIZE
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret


EscreverChar:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    mov edx,_CHARSIZE
    mov ecx,[ebp+8]
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_NWLNSIZE
    mov ecx,_nwln
    mov ebx,1
    mov eax,4
    int 80h
    
    pop ebp
    ret 4


LeerString:
    enter 0,0
    push ebx
    push ecx
    push edx
    
    mov eax,3
    mov ebx,0
    mov ecx,_string
    mov edx,_STRINGSIZE
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret


EscreverString:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    mov edx,_STRINGSIZE
    mov ecx,[ebp+8]
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_NWLNSIZE
    mov ecx,_nwln
    mov ebx,1
    mov eax,4
    int 80h
    
    pop ebp
    ret 4


_showQtdRead: 
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    add eax,0x30
    mov dword [_qtdChars],eax
    
    mov edx,_MSGQTDCHARSIZE
    mov ecx,_msgQtdChar
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_CHARSIZE
    mov ecx,_qtdChars
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_MSGQTDCHAR2SIZE
    mov ecx,_msgQtdChar2
    mov ebx,1
    mov eax,4
    int 80h
    mov edx,_NWLNSIZE
    mov ecx,_nwln
    mov ebx,1
    mov eax,4
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret