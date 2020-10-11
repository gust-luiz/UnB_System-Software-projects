#include "../include/twoPass.hpp"

// Função que inicializa a tabela de instruções (dicionário) com dados definidos para o montador
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

// Função para rodar a primeira passagem do montador
void TwoPass::runFirstPass(ifstream &inputFile){

    string line;
    string label, operation;
    int positionCounter = 0;
    map<string, Instruction*>::iterator it;  // Iterador para o dicionário da Tabela de Instruções

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

// Função para realização da segunda passagem do montador
void TwoPass::runSecondPass(ifstream &inputFile, ofstream &outputFile){

    string line, token;
    map<string, Instruction*>::iterator it;
    bool sectionText, sectionData;  // booleanos para indicar se a seção sendo percorrida é TEXT ou DATA

    while(getline(inputFile, line)){

        // Se a linha indicar início de SECTION, lê a linha seguinte e ativa o marcador correspondente à seção
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

        // Para linha dentro da seção de texto
        if (sectionText && !sectionData) {
            // Se linha contém rótulo, despreza o rótulo
            if(line.find(':') != string::npos){
                line = line.substr(line.find(':') + 1);
            }

            // Limpeza da linha para restar apenas tokens separados por espaços
            replace(line.begin(), line.end(), ',', ' ');
            stringstream ss(line);

            // Para cada token presente na linha
            while(ss >> token){
                // Se o token for uma instrução, grava na saída o opcode correspondente.
                // Se o token for um rótulo, obtém o valor da posição de memória na tabela de símbolos e grava na saída
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

        // Para linha dentro da seção de dados
        if(sectionData && !sectionText){
            // Se linha contém rótulo, despreza o rótulo
            if(line.find(':') != string::npos){
                line = line.substr(line.find(':') + 1);
            }

            stringstream ss(line);

            // Para cada token na linha
            while(ss >> token) {
                // Se o token for diretiva SPACE, grava 00 na saída.
                // Se o token for diretiva CONST, grava o valor correspondente na saída
                if(token == "SPACE"){
                    outputFile << "00" << " ";
                }
                if(token == "CONST"){
                    ss >> token;  // obtem valor correspondente ao CONST
                    outputFile << token << " ";
                }
            }
        }
    }
}

// Função base para rodar o algoritmo de duas passagens do montador
void TwoPass::runTwoPassAlgorithm(string filename){

    ifstream inputFile(filename);  // Abertura do arquivo de input
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".obj");  // Abertura do arquivo de output
    string line;

    createInstructions();  // Chama função para criar a Tabela de Instruções

    runFirstPass(inputFile);  // Primeira passagem
    // Exibir no console a tabela de símbolos gerada. Descomentar linha abaixo para visualizar a saída
    //printSymbolTable();

    // Reinicia o arquivo de input para rodar a Segunda passagem
    inputFile.clear();
    inputFile.seekg(0);
    
    runSecondPass(inputFile, outputFile);  // Segunda passagem

    // Fechamento dos arquivos de input e output
    inputFile.close();
    outputFile.close();
}


// Função para printar no console a tabela de símbolos gerada na Primeira passagem
void TwoPass::showSymbolTable() {
  for (auto symbol : symbolTable)
    cout << symbol->getLabel() << " | " << symbol->getValue() << endl;
}