#include "../include/translation.hpp"
#include "../include/utils.hpp"


// Função que inicializa a tabela de instruções (dicionário) com dados definidos para o montador
void Translation::createBackboneInstructions(){
    instructions = 
    {
        {"ADD", "add eax,[#1]"},
        {"SUB", "sub eax,[#1]"},
        {"MULT", "imul [#1]\npush edx\ncall _verificaOverflow\n"},
        {"DIV", "cdq eax\nidiv [#1]"},
        {"JMP", "jmp #1"},
        {"JMPN", "cmp eax,0\njl #1"},
        {"JMPP", "cmp eax,0\njg #1"},
        {"JMPZ", "cmp eax,0\nje #1"},
        {"COPY", "push eax\nmov eax,[#1]\nmov dword [#2],eax\npop eax"},
        {"LOAD", "mov eax, [#1]"},
        {"STORE", "mov dword [#1],eax"},
        {"INPUT", "push eax\npush #1\ncall LeerInteiro\ncall showQtdRead\npop eax"},
        {"OUTPUT", "push #1\ncall EscreverInteiro\ncall _printNewLine"},
        {"C_INPUT", "push eax\npush #1\ncall LeerChar\ncall _showQtdRead\npop eax"},
        {"C_OUTPUT", "push #1\ncall EscreverChar\ncall _printNewLine"},
        {"S_INPUT", "push eax\npush #1\npush #2\ncall LeerString\ncall _showQtdRead\npop eax"},
        {"S_OUTPUT", "push #1\npush #2\ncall EscreverString\ncall _printNewLine"},
        {"STOP", "call _EXIT"}        
    };
}

string Translation::fillInstructions(vector<string> args){
    string buffer;
    string operation = args[0];
    map<string, string>::iterator it;  // Iterador para o dicionário com backbone de instruções IA32

    it = instructions.find(operation);
    buffer = it->second;
    for (int i = 1; i <= args.size() ; i++) {
        findAndReplaceAll(buffer, "#"+to_string(i), args[i]);
    }

    return buffer;
}


void Translation::printSectionData(ofstream &outputFile, vector<string> dataVector){

    for (auto data : dataVector) {
        outputFile << data << endl;
    }

    return;
}


void Translation::printSectionBss(ofstream &outputFile, vector<string> bssVector){

    for (auto bss : bssVector) {
        outputFile << bss << endl;
    }

    return;
}


void Translation::printSectionText(ofstream &outputFile, vector<string> textVector){

    for (auto text : textVector) {
        outputFile << text << endl;
    }

    return;
}


void Translation::runTranslation(string filenamePre, string filenameFunc){

    ifstream inputFile(filenamePre);  // Abertura do arquivo de input
    ifstream inputFile2(filenameFunc);
    ofstream outputFile(filenamePre.substr(0, filenamePre.find('.')) + ".s");  // Abertura do arquivo de output
    string line, lineIA;
    vector<string> codeVector, bssVector, dataVector, textVector;


    bool sectionText, sectionData;  // booleanos para indicar se a seção sendo percorrida é TEXT ou DATA

    // Copiar as funções de Input/Output para o início do arquivo de saída em IA-32
    while(getline(inputFile2, line)){
        outputFile << line << endl;
    }
    inputFile2.close();

    while(getline(inputFile, line)){
        codeVector.push_back(line);
    }

    for (int i = 0; i < codeVector.size(); i++) {
        line = codeVector[i];

        if(line.find("SECTION DATA") != string::npos){
            i++;
            line = codeVector[i];
            sectionText = false;
            sectionData = true;
        }
        if(line.find("SECTION TEXT") != string::npos){
            i++;
            line = codeVector[i];
            sectionText = true;
            sectionData = false;
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

            label, directive = lineTokens[0], lineTokens[1];
            if(directive == "SPACE"){
                if (lineTokens.size() < 3){  //Linha space não apresenta value
                    value = '1';
                }
                lineIA = label + " resd " + value;
                bssVector.push_back(lineIA);
                
            }
            if(directive == "CONST"){
                value = lineTokens[2];  // obtem valor correspondente ao CONST
                lineIA = label + " dd " + value;
                dataVector.push_back(lineIA);
            }
            
        }

        if (sectionText && !sectionData) {
            vector<string> lineArgs;
            string token, label, directive, value;
            string newLine = line;

            label = "";
            // Verifica se a linha contém um rótulo
            if (line.find(':') != string::npos){
                size_t pos = line.find(':');
                label = line.substr(0,pos);
                newLine = line.substr(pos+1);
            }
            replace(newLine.begin(), newLine.end(), ',', ' ');  //substitui vírgulas por espaço para facilitar tokenização
            findAndReplaceAll(newLine, " + ", "+");  //remove espaços ao redor de + para facilitar tokenização

            stringstream ss(newLine);

            while(ss >> token){
                lineArgs.push_back(token);
            }
            lineIA = label + " " + fillInstructions(lineArgs);
            textVector.push_back(lineIA);
        }
    }

    printSectionData(outputFile, dataVector);
    printSectionBss(outputFile, bssVector);
    printSectionText(outputFile, textVector);

    // Fechamento dos arquivos de input e output
    inputFile.close();
    outputFile.close();

    return;

}