#include "../include/translation.hpp"

void runTranslation(string filename){
    vector<string> codeVector, bssVector, dataVector, textVector;

    ifstream inputFile(filename);  // Abertura do arquivo de input
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".s");  // Abertura do arquivo de output
    string line;

    while(getline(inputFile, line)){
        codeVector.push_back(line);
    }



    // Fechamento dos arquivos de input e output
    inputFile.close();
    outputFile.close();

    return;

}