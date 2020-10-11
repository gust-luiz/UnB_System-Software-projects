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

// Classe de símbolos a serem adicionados na Tabela de Símbolos
// Os objetos dessa Classe contém o rótulo e o valor correspondente ao seu endereço de memória
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

// Classe dos objetos que representam as intruções definidas do Montador.
// Os objetos contém o Opcode, o número de operandos e o tamanho de memória ocupado pela instrução
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

// Classe com funções e atributos necessários para realização do algoritmo de duas passagens do montador.
class TwoPass{
    public:
        void runTwoPassAlgorithm(string filename);
        void runFirstPass(ifstream &inputFile);
        void runSecondPass(ifstream &inputFile, ofstream &outputFile);

        void createInstructions();

        void showSymbolTable();

    private:
        // Dicionário que conterá o mnemônico da instrução como Key e um objeto da Classe Instruction como Value.
        map<string, Instruction*> instructions;   
        vector<Symbol*> symbolTable;  // Vetor que representa a Tabela de símbolos, com objetos da Classe Symbol
};

#endif