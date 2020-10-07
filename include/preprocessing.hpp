#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

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


class PreProcessing{
    public:
        PreProcessing(){}

        void runPreProcessing(string filename);

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
        vector<EquDirective*> equList;
        vector<MacroName*> MNT;
        vector<MacroDefinition*> MDT;
};

#endif