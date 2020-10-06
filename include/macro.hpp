#ifndef MACRO_H
#define MACRO_H

#include "preprocessing.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

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


class Macro: public PreProcessing{
    public:
        void macroDeclaration(ifstream& inputFile, string line);

        void printMNT();
        void printMDT();

    private:
        vector<MacroName*> MNT;
        vector<MacroDefinition*> MDT;
};

#endif