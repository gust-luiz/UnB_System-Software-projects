TAMSTRBASE EQU 30 
TAMSTRALTURA EQU 30 
TAMSTRRESULT EQU 20 
TAMSTREXIT EQU 20
TAMSHOWSTR EQU 5 ;tamanho para EscreverString - variar para verificar se exibe corretamente a quantidade

SECTION TEXT       
S_INPUT STRBASE, TAMSTRBASE ; escreva msg para solicitar o valor da base
S_INPUT STRALTURA, TAMSTRALTURA   ; escreva msg para solicitar o valor da altura
S_INPUT STRRESULT, TAMSTRRESULT ; escreva msg para quando for exibir o resultado
S_INPUT STREXIT, TAMSTREXIT ; escreva msg para solicitar um char para encerrar o programa

S_OUTPUT STRBASE, TAMSHOWSTR ; apresenta msg solicitando base
INPUT VALORES ; Lê a base B

S_OUTPUT STRALTURA, TAMSHOWSTR ; apresenta msg solicitando altura
INPUT VALORES + 1 ; Lê a altura H

LOAD VALORES ; Carrega B no acumulador
MULT VALORES + 1 ; ACC tem B*H
DIV DOIS ; ACC tem o resultado de (B+H)/2
STORE VALORES + 2 ; Coloca soma (B+H)/2 em R

S_OUTPUT STRRESULT, TAMSHOWSTR ; apresenta msg sobre o resultado
OUTPUT VALORES + 2 ; Escreve o resultado

S_OUTPUT STREXIT, TAMSTREXIT ; apresenta msg solicitando um caracter
C_INPUT CHAREXIT
C_OUTPUT    CHAREXIT ; exibe o caracter informado
STOP ; Termina a execução do programa

SECTION DATA
STRBASE: SPACE 25
STRALTURA: SPACE 25
STRRESULT: SPACE 25
STREXIT: SPACE 25
CHAREXIT: SPACE 1
VALORES: SPACE 3 ; 0:base 1:altura 2:resultado
DOIS: CONST 2 ; Reserva espaço para uma constante