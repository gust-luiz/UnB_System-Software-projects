#include "../include/translation.hpp"
#include "../include/utils.hpp"


// Função que inicializa a tabela de instruções (dicionário), com correspondência entre instruções do 
//assembly inventado e o assembly IA32
void Translation::createBackboneInstructions(){
    instructions = 
    {
        {"ADD", "add eax,[#1]"},
        {"SUB", "sub eax,[#1]"},
        {"MULT", "imul dword [#1]\npush edx\ncall _verificaOverflow"},
        {"DIV", "cdq\nidiv dword [#1]"},
        {"JMP", "jmp #1"},
        {"JMPN", "cmp eax,0\njl #1"},
        {"JMPP", "cmp eax,0\njg #1"},
        {"JMPZ", "cmp eax,0\nje #1"},
        {"COPY", "push eax\nmov eax,[#1]\nmov dword [#2],eax\npop eax"},
        {"LOAD", "mov eax, [#1]"},
        {"STORE", "mov dword [#1],eax"},
        {"INPUT", "push eax\npush #1\ncall LeerInteiro\ncall _showQtdRead\npop eax"},
        {"OUTPUT", "push #1\ncall EscreverInteiro\ncall _printNewLine"},
        {"C_INPUT", "push eax\npush #1\ncall LeerChar\ncall _showQtdRead\npop eax"},
        {"C_OUTPUT", "push #1\ncall EscreverChar\ncall _printNewLine"},
        {"S_INPUT", "push eax\npush #1\npush #2\ncall LeerString\ncall _showQtdRead\npop eax"},
        {"S_OUTPUT", "push #1\npush #2\ncall EscreverString\ncall _printNewLine"},
        {"STOP", "call _EXIT"}        
    };
}


//Função que recebe uma linha de instrução e substitui na instrução correspondente em IA32 indicada no dicionário
string Translation::fillInstructions(vector<string> args){
    string buffer;
    string operation = args[0];  //contém a operação para pesquisar nas keys do dicionário de instruções
    map<string, string>::iterator it;  // Iterador para o dicionário com backbone de instruções IA32

    it = instructions.find(operation);
    buffer = it->second;  //obtém o rascunho da instrução para substituir os parâmetros

    //Substituição dos parâmetros genéricos pelos reais
    for (size_t i = 1; i < args.size() ; i++) {
        findAndReplaceAll(buffer, "#"+to_string(i), args[i]);
    }

    return buffer;
}


//Função que imprime no arquivo de saída a seção .data
void Translation::printSectionData(ofstream &outputFile, vector<string> dataVector){
    outputFile << "section .data" << endl;
    for (auto data : dataVector) {
        outputFile << data << endl;
    }

    return;
}


//Função que imprime no arquivo de saída a seção .bss
void Translation::printSectionBss(ofstream &outputFile, vector<string> bssVector){
    outputFile << "section .bss" << endl;
    for (auto bss : bssVector) {
        outputFile << bss << endl;
    }

    return;
}


//Função que imprime no arquivo de saída a seção .text
void Translation::printSectionText(ofstream &outputFile, vector<string> textVector){
    outputFile << "section .text" << endl;
    outputFile << "global _start" << endl;
    outputFile << "_start: ";
    for (auto text : textVector) {
        outputFile << text << endl;
    }

    return;
}


// Função base para rodar a tradução
void Translation::runTranslation(string filenamePre, string filenameFunc){

    ifstream inputFile(filenamePre);  // Abertura do arquivo de input
    ifstream inputFile2(filenameFunc);
    ofstream outputFile(filenamePre.substr(0, filenamePre.find('.')) + ".s");  // Abertura do arquivo de output
    string line, lineIA;
    vector<string> codeVector, bssVector, dataVector, textVector;
    bool sectionText, sectionData;  // booleanos para indicar se a seção sendo percorrida é TEXT ou DATA

    createBackboneInstructions(); //criação do dicionário de instruções

    // Copiar as funções de Input/Output para o início do arquivo de saída em IA-32
    while(getline(inputFile2, line)){
        outputFile << line << endl;
    }
    inputFile2.close();

    // Transfere o conteúdo do arquivo de entrada para um vetor de código
    while(getline(inputFile, line)){
        codeVector.push_back(line);
    }
    inputFile.close();

    // Percorre o vetor de código
    for (size_t i = 0; i < codeVector.size(); i++) {
        line = codeVector[i];

        if(line.find("SECTION DATA") != string::npos){
            i++;
            line = codeVector[i];
            sectionText = false;
            sectionData = true;  // Se estiver na seção data irá setar as flags de acordo
        }
        if(line.find("SECTION TEXT") != string::npos){
            i++;
            line = codeVector[i];
            sectionText = true;
            sectionData = false;  // Se estiver na seção text irá setar as flags de acordo
        }

        // Para linha dentro da seção de dados
        if(sectionData && !sectionText){
            vector<string> lineTokens;
            string token, label, directive, value;

            // Substitui ':' após o rótulo para facilitar posterior obtenção de tokens
            replace(line.begin(), line.end(), ':', ' ');

            stringstream ss(line);
            while(ss >> token){
                lineTokens.push_back(token);
            }

            label = lineTokens[0];  //rótulo na seção data
            directive = lineTokens[1];  //diretiva da seção de dados
            if(directive == "SPACE"){
                if (lineTokens.size() < 3){  // Se linha space não apresenta value ele é definido como 1
                    value = '1';
                }else{
                    value = lineTokens[2];
                }
                lineIA = label + " resd " + value;  //variável double word
                bssVector.push_back(lineIA);  //SPACE vai para seção .bss no IA32
                
            }
            if(directive == "CONST"){
                value = lineTokens[2];  // obtém valor correspondente ao CONST
                lineIA = label + " dd " + value;  //variável double word
                dataVector.push_back(lineIA);  //CONST vai para seção .data no IA32
            }
            
        }

        // Para linha dentro da seção de texto
        if (sectionText && !sectionData) {
            vector<string> lineArgs;
            string token, label, directive, value;
            string newLine = line;

            label = "";  //inicializa label vazia pois pode estar presente ou não
            // Verifica se a linha contém um rótulo
            if (line.find(':') != string::npos){
                size_t pos = line.find(':');
                label = line.substr(0,pos+1) + " ";
                newLine = line.substr(pos+1);
            }
            replace(newLine.begin(), newLine.end(), ',', ' ');  //substitui vírgulas por espaço para facilitar tokenização
            findAndReplaceAll(newLine, " + ", "+");  //remove espaços ao redor de + para facilitar tokenização

            stringstream ss(newLine);

            while(ss >> token){
                if (token.find('+') != string::npos){
                    token = token + "*4";
                }
                lineArgs.push_back(token);
            }
            lineIA = label + fillInstructions(lineArgs);;
            textVector.push_back(lineIA);  //Adiciona no vetor de seção .text
        }
    }

    // chamada de funções para transferir o conteúdo dos vetores para o arquivo de saída
    printSectionData(outputFile, dataVector);
    printSectionBss(outputFile, bssVector);
    printSectionText(outputFile, textVector);

    // Fechamento dos arquivos de input e output
    outputFile.close();

    return;

}