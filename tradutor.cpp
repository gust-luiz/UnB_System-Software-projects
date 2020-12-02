/* 
Projeto 2 de Software Básico 1-2020 - Universidade de Brasília
Aluno: Luiz Gustavo Rodrigues Martins
Matricula: 13/0123293
*/

#include "include/preprocessing.hpp"
#include "include/translation.hpp"


int main(int argc, char **argv){

    if (argc != 2){
        cout << "Número incorreto de parâmetros!" << endl;
        cout << "Informe, como parâmetro, o arquivo .asm" << endl;
        exit(1);
    }

    string filename = argv[1];
    string filenamePre = filename.substr(0, filename.find('.')) + ".pre";
    string filenameFunc = "io_functions.asm";

    PreProcessing *preprocessing = new PreProcessing();
    preprocessing->runPreProcessing(filename);  // Pré-processamento

    Translation *translation = new Translation();
    translation->runTranslation(filenamePre, filenameFunc);   // Tradução para IA-32

    return 0;

}