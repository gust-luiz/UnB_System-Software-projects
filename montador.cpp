#include "include/preprocessing.hpp"
#include "include/twoPass.hpp"


int main(int argc, char **argv){

    if (argc != 3){
        cout << "Número incorreto de parâmetros!" << endl;
        exit(1);
    }

    string flagOption = argv[1];
    string filename = argv[2];

    PreProcessing *preprocessing = new PreProcessing();
    TwoPass *twoPass = new TwoPass();

    if (flagOption == "-p") {
        preprocessing->runPreProcessing(filename);  // Pré-processamento       
    }
    if (flagOption == "-o") {
        twoPass->runTwoPassAlgorithm(filename);  // Algoritmo de duas passagens do montador
    }

    return 0;

}