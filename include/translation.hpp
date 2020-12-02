#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <map>

using namespace std;

// Classe de Tradução. Com funções e atributos necessários para a realização da tradução para IA-32
class Translation{
    public:
        Translation(){}

        void runTranslation(string filenamePre, string filenameFunc);
        void createBackboneInstructions();
        string fillInstructions(vector<string> args);

        void printSectionData(ofstream &outputFile, vector<string> dataVector);
        void printSectionBss(ofstream &outputFile, vector<string> bssVector);
        void printSectionText(ofstream &outputFile, vector<string> textVector);

    private:
        map<string, string> instructions; 

};




#endif