section .data
_msgQtdChar    dd  'Foram lidos '
_MSGQTDCHARSIZE EQU  $-_msgQtdChar

_msgQtdChar2    dd  ' caracter(es)'
_MSGQTDCHAR2SIZE EQU  $-_msgQtdChar2

_msgLerInteiro  dd  'Informe um inteiro: '
_MSGLERINTEIROSIZE EQU  $-_msgLerInteiro

_msgLerChar dd  'Informe um caracter: '
_MSGLERCHARSIZE EQU  $-_msgLerChar

_msgLerString   dd  'Informe uma string: '
_MSGLERSTRINGSIZE EQU  $-_msgLerString

_nwln   dd	0dh,0ah
_NWLNSIZE	EQU	$-_nwln

_NUMBERSIZE	EQU	4
_CHARSIZE	EQU	1
_STRINGSIZE	EQU	100	


section .bss
_number resd    1
_char  resd   1 ;char armazenado como dword mas irá conter apenas um caracter
_string  resd   25  ; string contém até 25*4=100 caracteres
_stringSize  resd   1
_numberStr  resd   1    ;variável auxiliar para armazenar a string de inteiro lida para posterior conversão para inteiro
_qtdChars   resd    1   
_flagSigned resd    1
_dummyBuffer    resd    25


section .text
global _start
_start:
    
    push eax
    push _number   ;operando cujo endereço de memória irá armazenar o inteiro a ser lido
    call LeerInteiro
    call _showQtdRead
    pop eax
    
    push _number
    call EscreverInteiro
    call _printNewLine
    
    push eax
    push _char  ;operando cujo endereço de memória irá armazenar o char a ser lido
    call LeerChar
    call _showQtdRead
    pop eax
    
    push _char
    call EscreverChar
    call _printNewLine
    
    push eax
    push _string   ;operando cujo endereço de memória irá armazenar a string a ser lida
    push 100
    call LeerString
    call _showQtdRead
    pop eax
    
    push _string
    push 22
    call EscreverString
    call _printNewLine
    
    
    mov eax,1
    mov ebx,0
    int 80h

LeerInteiro:
    enter 0,0
    push ebx
    push ecx
    push edx
    push esi
    
    mov eax,4
    mov ebx,1
    mov ecx,_msgLerInteiro
    mov edx,_MSGLERINTEIROSIZE
    int 80h
    
    mov eax,3
    mov ebx,0
    mov ecx,_numberStr
    mov edx,_NUMBERSIZE
    int 80h
    
    

    ; se linefeed 0ah está no buffer, então ok e vai decrementar um em eax para excluir o 0ah da contagem de caracteres lidos
    cmp byte [ecx + eax - 1],0ah ; verifica LF no final do input lido
    jz _removeLFInt
    ; senão, irá colocar o restante do buffer do teclado em um buffer descartável (dummy)
    push eax
    mov eax,3
    mov ebx,0
    mov ecx, _dummyBuffer
    mov edx, 100
    int 80h
    pop eax
    jmp _continuaLeerInteiro
    
_removeLFInt:
    dec eax ;decrementa eax para não considerar o LF na contagem
    
_continuaLeerInteiro:
    push eax
    mov ecx,eax
    ;cmp ecx,1   ;ecx igual a 1 indica que há apenas [enter] como input
    ;je  _armazenaInteiro
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
    mov ebx,[ebp+8]
    mov [ebx],eax
    
    pop eax
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 4

    
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
    
    mov eax,4
    mov ebx,1
    mov ecx,_msgLerChar
    mov edx,_MSGLERCHARSIZE
    int 80h
    
    mov eax,3
    mov ebx,0
    mov ecx,[ebp+8];_char
    mov edx,_CHARSIZE
    int 80h
    
    ; se linefeed 0ah está no buffer, então ok e vai decrementar um em eax para excluir o 0ah da contagem de caracteres lidos
    cmp byte [ecx + eax - 1],0ah ; verifica LF no final do input lido
    jz _removeLFChar
    ; senão, irá colocar o restante do buffer do teclado em um buffer descartável (dummy)
    push eax
    mov eax,3
    mov ebx,0
    mov ecx, _dummyBuffer
    mov edx, 100
    int 80h
    pop eax
    jmp _fimLeerChar
    
_removeLFChar:
    dec eax ;decrementa eax para não considerar o LF na contagem
    
_fimLeerChar:
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 4


EscreverChar:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx

    mov eax,4
    mov ebx,1
    mov ecx,[ebp+8]
    mov edx,_CHARSIZE
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret 4


LeerString:
    enter 0,0
    push ebx
    push ecx
    push edx
    
    mov eax,4
    mov ebx,1
    mov ecx,_msgLerString
    mov edx,_MSGLERSTRINGSIZE
    int 80h
    
    mov eax,3
    mov ebx,0
    mov ecx,[ebp+12];_string
    mov edx,[ebp+8];_STRINGSIZE
    int 80h
    
    ; se linefeed 0ah está no buffer, então ok e vai decrementar um em eax para excluir o 0ah da contagem de caracteres lidos
    cmp byte [ecx + eax - 1],0ah ; verifica LF no final do input lido
    jz _removeLFString
    ; senão, irá colocar o restante do buffer do teclado em um buffer descartável (dummy)
    push eax
    mov eax,3
    mov ebx,0
    mov ecx, _dummyBuffer
    mov edx, 100
    int 80h
    pop eax
    jmp _fimLeerString
    
_removeLFString:
    dec eax ;decrementa eax para não considerar o LF na contagem
    
_fimLeerString:
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 8


EscreverString:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    mov eax,4
    mov ebx,1
    mov ecx,[ebp+12]
    mov edx,[ebp+8]
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret 8


_showQtdRead: 
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    mov dword [_qtdChars],eax

    mov eax,4
    mov ebx,1
    mov ecx,_msgQtdChar
    mov edx,_MSGQTDCHARSIZE
    int 80h
    
    push _qtdChars
    call EscreverInteiro
    
    mov eax,4
    mov ebx,1
    mov ecx,_msgQtdChar2
    mov edx,_MSGQTDCHAR2SIZE
    int 80h
    
    mov eax,4
    mov ebx,1
    mov ecx,_nwln
    mov edx,_NWLNSIZE
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret
    
    
_printNewLine:
    enter 0,0
    push eax
    push ebx
    push ecx
    push edx
    
    mov eax,4
    mov ebx,1
    mov ecx,_nwln
    mov edx,_NWLNSIZE
    int 80h
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret
