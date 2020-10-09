#include "../include/twoPass.hpp"


void TwoPass::createInstructions(){
    instructions = 
    {
        {"ADD", new Instruction("01", 1, 2)},
        {"SUB", new Instruction("02", 1, 2)},
        {"MULT", new Instruction("03", 1, 2)},
        {"DIV", new Instruction("04", 1, 2)},
        {"JMP", new Instruction("05", 1, 2)},
        {"JMPN", new Instruction("06", 1, 2)},
        {"JMPP", new Instruction("07", 1, 2)},
        {"JMPZ", new Instruction("08", 1, 2)},
        {"COPY", new Instruction("09", 2, 3)},
        {"LOAD", new Instruction("10", 1, 2)},
        {"STORE", new Instruction("11", 1, 2)},
        {"INPUT", new Instruction("12", 1, 2)},
        {"OUTPUT", new Instruction("13", 1, 2)},
        {"STOP", new Instruction("14", 0, 1)}        
    };
}


void TwoPass::runFirstPass(ifstream &inputFile){

    string line;
    string label, operation;
    int positionCounter = 0;
    map<string, Instruction*>::iterator it;

    while(getline(inputFile, line)){

        // Se for linha de SECTION continua para leitura da próxima linha
        if(line.find("SECTION") != string::npos){
            continue;
        }

        // Se linha contém rótulo (caracter :)
        if(line.find(":") != string::npos){
            replace(line.begin(), line.end(), ':', ' ');
            replace(line.begin(), line.end(), ',', ' ');

            stringstream ss(line);

            // Obtém label e mnemônico da operação
            ss >> label >> operation;

            // Cadastra o símbolo e sua posição em memória na Tabela de Símbolos
            Symbol *symbol = new Symbol(label, to_string(positionCounter));
            symbolTable.push_back(symbol);

            // Verifica na tabela de instruções se a operação corresponde à uma instrução.
            // Se for instrução, atualiza contador de posição de acordo com tamanho da instrução
            it = instructions.find(operation);
            if (it != instructions.end()){
                positionCounter += it->second->getSize();
            } else {
                // Diretivas CONST e SPACE incrementam contador de posição em 1
                if(operation == "CONST" || operation == "SPACE"){
                    positionCounter += 1;
                }
            }


        } else {  // Se não possuir rótulo, obtém diretamente a operação
            stringstream ss(line);
            ss >> operation;

            // Atualiza o contador de posição de acordo com o tamanho da instrução ou diretiva
            it = instructions.find(operation);
            if (it != instructions.end()){
                positionCounter += it->second->getSize();
            }
        }
    }
}


void TwoPass::runSecondPass(ifstream &inputFile, ofstream &outputFile){

    string line, token;
    map<string, Instruction*>::iterator it;
    bool sectionText, sectionData;

    while(getline(inputFile, line)){

        if(line.find("SECTION TEXT") != string::npos){
            getline(inputFile, line);
            sectionText = true;
            sectionData = false;
        }
        if(line.find("SECTION DATA") != string::npos){
            getline(inputFile, line);
            sectionText = false;
            sectionData = true;
        }

        if (sectionText && !sectionData) {
            if(line.find(':') != string::npos){
                line = line.substr(line.find(':') + 1);
            }

            replace(line.begin(), line.end(), ',', ' ');
            stringstream ss(line);

            while(ss >> token){

                it = instructions.find(token);
                if (it != instructions.end()){
                    outputFile << it->second->getOpcode() << " ";
                } 
                else {
                    for(auto symbol : symbolTable){
                        if(symbol->getLabel() == token){
                            outputFile << symbol->getValue() << " ";
                        }
                    }

                }
            }
        }

        if(sectionData && !sectionText){
            if(line.find(':') != string::npos){
                line = line.substr(line.find(':') + 1);
            }

            stringstream ss(line);

            while(ss >> token) {

                if(token == "SPACE"){
                    outputFile << "00" << " ";
                }
                if(token == "CONST"){
                    ss >> token;
                    outputFile << token << " ";
                }
            }
        }
    }
}


void TwoPass::runTwoPassAlgorithm(string filename){

    ifstream inputFile(filename);
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".obj");
    string line;

    createInstructions();

    runFirstPass(inputFile);
    //printSymbolTable();

    inputFile.clear();
    inputFile.seekg(0);
    
    runSecondPass(inputFile, outputFile);

    inputFile.close();
    outputFile.close();
}


void TwoPass::printSymbolTable() {
  for (auto symbol:symbolTable)
    cout << "Symbol: " << symbol->getLabel() << " | Value: " << symbol->getValue() << endl;
}