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

class PreProcessing{
    public:
        PreProcessing(){}

        string lineUpperCase(string line);
        string lineCleaning(string line);
        string lineCleanExtraBlanks(string line);
        string lineCleanComments(string line);

        void runPreProcessing(string filename);

        void equDeclaration(string line);
        string checkEqu(string line);

        void ifClause(ifstream &inputFile, string line);

        void printEquList();

    private:
        vector<EquDirective*> equList;
};

#endif