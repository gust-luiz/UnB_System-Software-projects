SECTION TEXT

S_INPUT STRPOSITIVO,10
S_INPUT STRNEGATIVO,10
S_INPUT STRZERO,10

INPUT VALORES + 1
INPUT VALORES

Multiplicacao:LOAD VALORES + 1
MULT VALORES
STORE RESULTADOS
OUTPUT  RESULTADOS
JMPP multpositivo
JMPN multnegativo
JMPZ multzero

multpositivo:
S_OUTPUT STRPOSITIVO,10
JMP adicao

multnegativo:
S_OUTPUT STRNEGATIVO,10
JMP adicao

multzero:
S_OUTPUT STRZERO,10
JMP adicao

ADICAO:
LOAD VALORES + 1
ADD VALORES
STORE RESULTADOS + 1
OUTPUT RESULTADOS + 1
JMPP ADDpositivo
JMPN ADDnegativo
JMPZ ADDzero

ADDpositivo:
S_OUTPUT STRPOSITIVO,10
JMP subtracao

ADDnegativo:
S_OUTPUT STRNEGATIVO,10
JMP subtracao

ADDzero:
S_OUTPUT STRZERO,10
JMP subtracao

subtracao:
LOAD VALORES + 1
SUB VALORES
STORE RESULTADOS + 2
OUTPUT RESULTADOS + 2
JMPP SUBpositivo
JMPN SUBnegativo
JMPZ SUBzero

SUBpositivo:
S_OUTPUT STRPOSITIVO,10
JMP DIVISAO

SUBnegativo:
S_OUTPUT STRNEGATIVO,10
JMP DIVISAO

SUBzero:
S_OUTPUT STRZERO,10
JMP DIVISAO

DIVISAO:
LOAD VALORES + 1
DIV VALORES
STORE RESULTADOS + 3
OUTPUT RESULTADOS + 3
JMPP divpositivo
JMPN DIVnegativo
JMPZ DIVzero

divpositivo:
S_OUTPUT STRPOSITIVO,10
JMP TESTECOPY

DIVnegativo:
S_OUTPUT STRNEGATIVO,10
JMP TESTECOPY

DIVzero:
S_OUTPUT STRZERO,10
JMP TESTECOPY

TESTECOPY:
OUTPUT VALORES+1
OUTPUT VALORES
COPY VALORES, VALORES + 1
OUTPUT VALORES + 1
OUTPUT VALORES

FIM:
C_INPUT CHAREXIT
C_OUTPUT CHAREXIT
STOP

SECTION DATA
STRPOSITIVO: SPACE 25
STRNEGATIVO: SPACE 25
STRZERO: SPACE 25
CHAREXIT: SPACE 1
VALORES: SPACE 2 ; 0:valor1 1:valor2
RESULTADOS: SPACE 4
