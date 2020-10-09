#ifndef TWOPASS_H
#define TWOPASS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;


class Symbol{
    public:
        Symbol();
        Symbol(string label, string value){
            setLabel(label);
            setValue(value);
        };

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
        string label;
        string value;
};


class Instruction{
    public:
        Instruction();
        Instruction(string opcode, int numOperands, int size){
            setOpcode(opcode);
            setNumOperands(numOperands);
            setSize(size);
        }

        void setOpcode(string opcode){
            this->opcode = opcode;
        }

        void setNumOperands(int numOperands){
            this->numOperands = numOperands;
        }

        void setSize(int size){
            this->size = size;
        }

        string getOpcode(){
            return this->opcode;
        }

        int getNumOperands(){
            return this->numOperands;
        }

        int getSize(){
            return this->size;
        }

    private:
        string opcode;
        int numOperands, size;
};


class TwoPass{
    public:
        void runTwoPassAlgorithm(string filename);
        void runFirstPass(ifstream &inputFile);
        void runSecondPass(ifstream &inputFile, ofstream &outputFile);

        void createInstructions();

        void printSymbolTable();

    private:
        map<string, Instruction*> instructions;
        vector<Symbol*> symbolTable;
};

#endif