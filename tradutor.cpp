/* 
Projeto 2 de Software Básico 1-2020 - Universidade de Brasília
Aluno: Luiz Gustavo Rodrigues Martins
Matricula: 13/0123293
*/

#include "include/preprocessing.hpp"


int main(int argc, char **argv){

    if (argc != 2){
        cout << "Número incorreto de parâmetros!" << endl;
        cout << "Informe, como parâmetro, o arquivo .asm" << endl;
        exit(1);
    }

    string filename = argv[1];
    string filenamePre = filename.substr(0, filename.find('.')) + ".pre";

    PreProcessing *preprocessing = new PreProcessing();

    preprocessing->runPreProcessing(filename);  // Pré-processamento       

    return 0;

}