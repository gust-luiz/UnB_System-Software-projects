;-----------------------------------------------
;Trecho com funções de Input/Output

section .data
_msgQtdChar    dd  'Foram lidos '
_MSGQTDCHARSIZE EQU  $-_msgQtdChar

_msgQtdChar2    dd  ' caracter(es)'
_MSGQTDCHAR2SIZE EQU  $-_msgQtdChar2

_msgOverflow    dd  'Ocorreu overflow na multiplicação! O programa foi encerrado.'
_MSGOVERFLOWSIZE    EQU $-_msgOverflow

_nwln   dd	0dh,0ah
_NWLNSIZE	EQU	$-_nwln

_NUMBERSIZE	EQU	4 ;o inteiro de entrada de 3 dígitos contém no máximo 4 caracteres considerando o sinal negativo
_CHARSIZE	EQU	1 ;char contém apenas um caracter de tamanho


section .bss
_numberStr  resd   3    ;variável auxiliar para armazenar uma string de inteiro de 32 bits
_qtdChars   resd    1   ;variável que irá conter a quantidade de caracteres lidos
_flagSigned resd    1   ;variável cujo valor irá indicar se um determinado número é negativo ou não
_dummyBuffer    resd    25  ;variável para descartar caracteres em excesso que permanecem no buffer do teclado


section .text
; rotina LeerInteiro
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
    sub eax,eax
    sub ebx,ebx
    mov esi,0
    cmp byte [_numberStr],'-'  ;verifica se o número é negativo '-'
    jne _converterStringInteiro
    inc esi ;se o número for negativo, já incrementa o esi que indica o index do caracter
    mov dword [_flagSigned],1  ;seta flag indicando que o número é negativo
_converterStringInteiro:
    mov bl,[_numberStr+esi]
    sub ebx,0x30 ;transforma para representação de inteiro
    add eax,ebx
    inc esi
    cmp byte [_numberStr+esi],0ah ;LF [enter] é um indicativo de que terminou o número
    je  _verificaNegativo
    cmp esi,ecx ;outro indicativo de que o número terminou é verificar o ecx que contém a quantidade de dígitos lidos
    je  _verificaNegativo
    mov ebx,10
    imul ebx
    jmp _converterStringInteiro
_verificaNegativo:
    cmp dword [_flagSigned],0
    je  _armazenaInteiro
    neg eax ;o número contido em eax é convertido para negativo
    mov dword [_flagSigned],0 ;reseta a flag de negativo
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
    mov dword [_numberStr],0
    test eax,eax 
    jns _converterInteiroString ;após o 'test' verifica a flag de signed para checar se o número é negativo ou não
    mov dword [_flagSigned],1 ;seta a flag se o número for negativo
    neg eax ;nega o número para realizar operação com ele positivo
    mov dword [_numberStr+esi],'-' ;já insere o sinal de negativo
    inc esi
_converterInteiroString:
    mov ecx,10
    cdq
    idiv ecx
    add edx,0x30    ;converte o resto binário em edx para ASCII
    push edx
    cmp eax,0   ;compara se o quociente binário em eax é igual a zero
    jne _notZero
    jmp _incremento
_notZero:
    mov ecx,esi
    inc ecx ; ecx vale esi+1
    mov edi,[_numberStr+esi]
    mov dword [_numberStr+ecx],edi
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
_recuperaDigitos: ;os dígitos foram inserido na pilha e são resgatados para manter a ordem dos dígitos
    pop dword [_numberStr+esi]
    inc esi
    cmp esi,ecx
    jne _recuperaDigitos
    mov edx,esi ;esi contém a quantidade de dígitos
    mov ecx,_numberStr
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

;rotina para leitura de um char
LeerChar:
    enter 0,0
    push ebx
    push ecx
    push edx
    
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

;rotina para escrever um char
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

;rotina para leitura de string
LeerString:
    enter 0,0
    push ebx
    push ecx
    push edx
    
    mov eax,3
    mov ebx,0
    mov ecx,[ebp+12];
    mov edx,[ebp+8];
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

;rotina para exibir string
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
    
;rotina para mostrar quantidade de caracteres que foram lidos em uma função de LER (input)
;a quantidade de caracteres está contida no registrador eax
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
    call EscreverInteiro ;chama função EscreverInteiro para exibir a quantidade de inteiros
    
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
   
;rotina para verificação de Overflow
;o registrador edx passado como parâmetro é comparado com 0x00000000 e 0xFFFFFFFF, sendo diferente indica overflow
_verificaOverflow:
    enter 0,0 
    cmp dword [ebp+8],0x00000000
    je _falseOverflow
    cmp dword [ebp+8],0xFFFFFFFF
    je _falseOverflow
;se ocorrer overflow irá exibir mensagem e pular para rotina de encerramento do programa
_trueOverflow:
    mov eax,4
    mov ebx,1
    mov ecx,_msgOverflow
    mov edx,_MSGOVERFLOWSIZE
    int 80h
    jmp _EXIT
;se não ocorreu overflow retorna para continuação do programa
_falseOverflow:
    pop ebp
    ret 4
    
;rotina para printar [enter] no display
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
    
; rotina com interrupção para encerrar o programa
_EXIT:
    mov eax,1
    mov ebx,0
    int 80h
;-----------------------------------------------