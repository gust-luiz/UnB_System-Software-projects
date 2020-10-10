#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>


using namespace std;


// Classe de diretivas EQU. Os objetos possuem os atributos de rótulo e o valor definido na diretiva.
class EquDirective{
    public:
        void setLabel(string label){
            this->label = label;
        }

        void setValue(string value){
            this->value = value;
        }

        string getLabel(){
            return this->label;
        }

        string getValue(){
            return this->value;
        }

    private:
        string label, value;

};

// Classe para objetos que serão adicionados a MNT. Os objetos possuem o nome da macro, a quantidade
// de argumentos e o índice na MDT que conterá o corpo da definição da macro.
class MacroName{
    public:
        void setName(string name){
            this->name = name;
        }

        void setNumArgs(int numArgs){
            this->numArgs = numArgs;
        }

        void setIndexMDT(int indexMDT){
            this->indexMDT = indexMDT;
        }

        string getName(){
            return this->name;
        }

        int getNumArgs(){
            return this->numArgs;
        }

        int getIndexMDT(){
            return this->indexMDT;
        }

    private:
        string name;
        int numArgs, indexMDT;
};

// Classe de objetos que serão adicionados na MDT. Os objetos guardam a definição de macros.
class MacroDefinition{
    public:
        void setDefinition(string definition){
            this->definition = definition;
        }

        string getDefinition(){
            return this->definition;
        }

    private:
        string definition;
};

// Classe de pré-processamento. Com funções e atributos necessários para a realização do pré-processamento
class PreProcessing{
    public:
        PreProcessing(){}

        void runPreProcessing(string filename);

        void lineFormatting(ifstream &inputFile, string &line);

        void diretivesPipeline(ifstream &inputFile, ofstream &outputFile, string &line);

        void macroDeclaration(ifstream &inputFile, ofstream &outputFile, string &line);
        void equDeclaration(string line);

        void checkMacro(ofstream &outputFile, string &line);
        string checkEqu(string &line);
        void ifClause(ifstream &inputFile, string &line);

        void printEquList();
        void printMNT();
        void printMDT();

    private:
        vector<EquDirective*> equList;  // Vetor que conterá as diretivas EQU
        vector<MacroName*> MNT;  // Vetor que conterá objetos da MNT (Macro Name Table)
        vector<MacroDefinition*> MDT;  // Vetor que conterá objetos da MDT (Macro Definition Table)
};

#endif