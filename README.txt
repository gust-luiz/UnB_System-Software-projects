Projeto 1 da disciplina de Software Básico 1º/2020 ministrada pelo professor Bruno Macchiavello - Universidade de Brasília

**Aluno: Luiz Gustavo Rodrigues Martins
**Matrícula: 13/0123293

- Observação: Foi implementada a proposta de o Montador aceitar a diretiva MACRO.

- O problema proposto: Implementar um Assembler (montador) da linguagem hipotética vista em sala de aula.

- Objetivo: Fixar o funcionamento de um processo de tradução com pré-processamento e algoritmo de duas passagens.



- AMBIENTE DE DESENVOLVIMENTO:
O projeto foi implementado em linguagem C++ (-std=C++11) no Ubuntu 18.04.
Versão do gcc/g++ utilizado: gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0 / g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0



- ESTRUTURA DE ARQUIVOS E PASTAS:
include: contém os arquivos header .hpp do programa;
src: contém os códigos fonte .cpp do programa;
obj: pasta será criada após compilação do programa, armazenando os códigos objeto do programa;
makefile: arquivo para agrupar comandos de compilação do programa, facilitando essa etapa.
simulador: executável fornecido pelo professor da disciplina para testes dos arquivos .obj;
montador.cpp: arquivo principal do programa.

Os arquivos .pre e .obj serão gerados na mesma pasta onde se encontra o montador.cpp.



- COMPILAÇÃO E EXECUÇÃO:
1) Limpar arquivos gerados em alguma compilação/execução anterior:
Comando: 
	make clean

2) Compilar o programa:
Comando:
	make

3) Execução do programa:
3.1) Comando para pré-processamento, gerando saída com extensão .pre:
	./montador -p <arquivo_entrada>.asm

3.2) Comando para montagem, gerando saída com extensão .obj:
	./montador -o <arquivo_entrada>.pre



- SIMULADOR:
Foi fornecido pelo professor da disciplina, um simulador para testes dos arquivos .obj gerados pelo programa implementado.
Comando para utilizar o simulador:
	./simulador <arquivo_entrada>.obj


----------------------------------------------------------------------------------------------------------------------------------


Projeto 2 da disciplina de Software Básico 1º/2020 ministrada pelo professor Bruno Macchiavello - Universidade de Brasília

**Aluno: Luiz Gustavo Rodrigues Martins
**Matrícula: 13/0123293

- Observações:
1) Na contagem da quantidade de caracteres lidos está sendo desconsiderado o [enter];
2) O segundo operando das funções de input e output de string é referente ao tamanho.

- O problema proposto: Implementar um tradutor para converter instruções do assembly inventado para o assembly IA32.

- Objetivo: Fixar o funcionamento de um processo de tradução com pré-processamento.



- AMBIENTE DE DESENVOLVIMENTO:
O projeto foi implementado em linguagem C++ (-std=C++11) no Ubuntu 18.04.
Versão do gcc/g++ utilizado: gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0 / g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0



- ESTRUTURA DE ARQUIVOS E PASTAS:
include: contém os arquivos header .hpp do programa;
src: contém os códigos fonte .cpp do programa;
obj: pasta será criada após compilação do programa, armazenando os códigos objeto do programa;
makefile: arquivo para agrupar comandos de compilação do programa, facilitando essa etapa;
io_functions.asm: arquivo que contém funções de INPUT/OUTPUT implementadas, cujo conteúdo será copiado para o arquivo .s de saída do tradutor;
tradutor.cpp: programa principal para realização da tradução;




- COMPILAÇÃO E EXECUÇÃO:
1) Limpar arquivos gerados em alguma compilação/execução anterior:
Comando: 
	make clean

2) Compilar o programa:
Comando:
	make

3) Execução do programa:
3.1) Comando para tradução, gerando saída com extensão .s. 
Também gera saída .pre (pré-processamento) que é utilizada durante execução:
	./tradutor <arquivo>.asm

3.2) Comando para montagem, usando o NASM:
	nasm -f elf -o <arquivo>.o <arquivo>.s

3.3) Geração do executável com o ligador ld:
	ld -m elf_i386 -o <arquivo> <arquivo>.o

3.4) Utilizar o executável:
	./<arquivo>
