#include "../include/preprocessing.hpp"
#include "../include/utils.hpp"

bool removeLine, declaration;


// Função para formatar corretamente determinada linha, limpando espaços, colocando em caixa alta
// e removendo quebras de linha desnecessárias. Se necessário, agrupa linhas em uma única linha
void PreProcessing::lineFormatting(ifstream &inputFile, string &line){

    string nextLine;

    line = lineCleaning(line);

    removeLine = false;

    // Linhas vazias ou com apenas espaços em branco são descartadas
    while (line.empty() || all_of(line.begin(), line.end(), [](char c){return isspace(c);})) {
        if (getline(inputFile, line)){
            line = lineCleaning(line);
        } else {
            removeLine = true;
            break;
        }
    }

    line = lineUpperCase(line);

    // Se a linha contém um rótulo
    if (line.find(':') != string::npos){
        size_t pos = line.find(':');
        string aux = line.substr(pos+1);

        // Verifica se há quebra de linha após rótulo e busca próxima linha com conteúdo para esse rótulo
        if (aux.empty() || (aux.compare(" ") == 0)){
            getline(inputFile, nextLine);
            nextLine = lineCleaning(nextLine);

            while (line.empty() || all_of(nextLine.begin(), nextLine.end(), [](char c){return isspace(c);})) {
                if(getline(inputFile, nextLine)){
                    nextLine = lineCleaning(nextLine);
                } else{
                    break;
                }
            }
            nextLine = lineUpperCase(nextLine);

            // Concatena linha com conteúdo ao rótulo no qual ocorreu quebra de linha, definindo uma única linha
            line += " " + nextLine;
        }
    }
}

// Função para leitura e armazenamento da definição de uma macro na MNT
void PreProcessing::macroDeclaration(ifstream &inputFile, ofstream &outputFile, string &line){

    string label, token, newLine;
    string buffer = "";
    vector<string> argsList;  // Vetor que conterá os argumentos da definição da macro

    // Limpeza da linha para restar apenas tokens separados por espaços
    replace(line.begin(), line.end(), ':', ' ');
    replace(line.begin(), line.end(), ',', ' ');
    line.erase(line.find(" MACRO"), 6);

    stringstream ss(line);

    bool getLabel = false;
    // Obtém o rótulo da macro e os argumentos caso existam
    while(ss >> token){
        if (!getLabel){
            label = token;
            getLabel = true;
        } else{
            argsList.push_back(token);
        }
    }

    // Cria uma objeto MacroName e cadastra na MNT essa intância que contém o nome,
    // quantidade de argumentos e o índice na MDT que irá conter a definição da macro
    MacroName *macroName = new MacroName();
    macroName->setName(label);
    macroName->setNumArgs(argsList.size());
    macroName->setIndexMDT(MDT.size());
    MNT.push_back(macroName);

    // Loop para obtenção da definição do corpo da Macro
    while(getline(inputFile, line)){

        lineFormatting(inputFile, line);

        // Se chegar ao final da definição da Macro, retorna
        if(line.find("ENDMACRO") != string::npos){
            buffer += line;
            break;
        }

        // Verificação de diretivas de pré-processamento no interior da definição da Macro
        diretivesPipeline(inputFile, outputFile, line);

        // Caso contenha alguma diretiva IF no interior da definição, ela terá sido processada
        // na verificação de diretiva e a linha da sentença IF não aparecerá na definição
        if(removeLine){
            continue;
        }

        newLine = line;
        // Limpeza da linha para conter apenas tokens separados por espaços
        replace(newLine.begin(), newLine.end(), ':', ' ');
        replace(newLine.begin(), newLine.end(), ',', ' ');

        stringstream ss1(newLine);
        // Para cada token, verifica se é argumento da Macro por meio do caracter '&'. Então substitui esse
        // caracter pelo índice correspondente ao argumento
        while(ss1 >> token){
            if (token.find('&') != string::npos){
                for (size_t i = 0; i < argsList.size(); i++){
                    if(token == argsList[i]){
                        line = line.replace(line.find(token), token.size(), to_string(i));
                    }
                }
            }
        }
        // Vai preenchendo o buffer que conterá a definição da Macro
        buffer += line + "\n";
    }

    // Cria uma nova instância de definição de Macro e cadastra na MDT
    MacroDefinition *macroDefinition = new MacroDefinition();
    macroDefinition->setDefinition(buffer);
    MDT.push_back(macroDefinition);
}

// Função para verificar se uma linha contém chamada de macro.
// Caso possua, irá expandir com a definição obtida da MDT para a macro em questão
void PreProcessing::checkMacro(ofstream &outputFile, string &line){

    string token;
    string newLine = line;
    string buffer;

    // Limpeza da linha para restar apenas tokens separados por espaços
    replace(newLine.begin(), newLine.end(), ':', ' ');
    replace(newLine.begin(), newLine.end(), ',', ' ');

    stringstream ss(newLine);

    // Para cada token, verifica se há uma macro com nome correspondente na MNT
    while(ss >> token){
        for (vector<MacroName*>::iterator it = MNT.begin(); it != MNT.end(); it++) {
            if ((*it)->getName() == token) {
                // Com macro correspondente, obtém a definição cadastrada na MDT de acordo com o índice
                buffer = MDT[(*it)->getIndexMDT()]->getDefinition();
                buffer.erase(buffer.find("ENDMACRO"), 8);

                // Leitura dos tokens restantes (que são os argumentos da macro) e substitui os argumentos
                // genéricos da definição obtida da MDT pelos argumento reais
                for (int i = 0; i < (*it)->getNumArgs(); i++) {
                    ss >> token;
                    findAndReplaceAll(buffer, to_string(i), token);
                }

                // Remover \n do final do buffer que contém a definição da macro com os argumentos corretos
                buffer.pop_back();

                // Linha a ser escrita será o buffer que contém a definição da macro
                line = buffer;
                break;
            }
        }
    }
}

// Função para obtenção de uma diretiva EQU
void PreProcessing::equDeclaration(string line){

    string label, value;

    // Limpeza da linha para restar apenas o rótulo e seu valor separados por espaços
    replace(line.begin(), line.end(), ':', ' ');
    line.erase(line.find(" EQU"), 4);

    stringstream ss(line);

    // Obtenção dos tokens referentes ao rótulo e ao seu valor correspondente
    ss >> label >> value;

    // Cria uma nova Diretiva EQU relacionando o rótulo ao valor e adiciona no vetor de EQU's
    EquDirective *equDirective = new EquDirective();
    equDirective->setLabel(label);
    equDirective->setValue(value);
    equList.push_back(equDirective);
}

// Função que verifica se a linha contém um rótulo de uma diretiva EQU e o substitui devidamente
string PreProcessing::checkEqu(string &line){

    string token, value;
    string newLine = line;

    // Limpeza da linha para manter os tokens de texto separados por espaço
    replace(newLine.begin(), newLine.end(), ':', ' ');
    replace(newLine.begin(), newLine.end(), ',', ' ');

    stringstream ss(newLine);

    // Para cada token da linha, verifica se há correspondente no vetor de EQU's. Caso haja, substitui o token pelo valor
    // correspondente ao EQU. Caso não haja, a linha não é alterada.
    while(ss >> token){
        for (vector<EquDirective*>::iterator it = equList.begin(); it != equList.end(); it++) {
            if((*it)->getLabel() == token){
                value = ((*it)->getValue());
                line = line.replace(line.find(token), token.size(), value);
                break;
            }
        }
    }

    return line;
}

// Função para verificação de uma sentença IF.
void PreProcessing::ifClause(ifstream &inputFile, string &line) {

    string ifExpression;
    int ifValue;

    // Se houver rótulo na linha com a sentença IF, ele será desprezado
    if(line.find(':') != string::npos){
        line = line.substr(line.find(':') + 1);
    }
    // Limpa o texto "IF" para restar apenas a expressão
    line.erase(line.find("IF "), 3);

    stringstream ss(line);
    
    // Obtém a expressão da sentença IF
    ss >> ifExpression;

    // Percorre vetor com as diretivas EQU para obter o valor referente à expressão da sentença IF
    for (vector<EquDirective*>::iterator it = equList.begin(); it != equList.end(); it++) {
        if((*it)->getLabel() == ifExpression){
            ifValue = stoi((*it)->getValue());
            break;
        }
    }
    
    // Se o valor da expressão for maior que zero, retorna para permitir leitura da próxima linha
    if(ifValue > 0){
        return;
    }

    // Se a expressão não for maior que zero, lê a próxima linha para descarte linha e retorna
    getline(inputFile, line);
    return;
}

// Função com sequência de ações (pipeline) de verificação que uma linha do arquivo irá passar
void PreProcessing::diretivesPipeline(ifstream &inputFile, ofstream &outputFile, string &line){
    
    smatch matches;
    regex reg;

    removeLine = false;
    declaration = false;

    // Declaração de Macro
    if (regex_search(line, matches, reg = ("([: ](MACRO)(\\s|$))"))){
        macroDeclaration(inputFile, outputFile, line);
        removeLine = true;
        declaration = true;
    }

    // Declaração de diretiva EQU
    if (regex_search(line, matches, reg = ("([: ](EQU ))"))){
        equDeclaration(line);
        removeLine = true;
        declaration = true;
    }

    // Sentença IF
    if (regex_search(line, matches, reg = ("((^|:|\\s)IF(\\s))"))){
        ifClause(inputFile, line);
        removeLine = true;
    }

    // Se a linha não for de declaração de Macro ou EQU
    if (!declaration) {
        checkMacro(outputFile, line);  // verifica se a linha contém chamada de Macro
        checkEqu(line);  // verifica se a linha contém rótulo referente a um EQU
    }
    
}

// Função base para rodar o pré-processamento
void PreProcessing::runPreProcessing(string filename){

    ifstream inputFile(filename);  // Abertura do arquivo de input
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".pre");  // Abertura do arquivo de output
    string line;

    while(getline(inputFile, line)){
        lineFormatting(inputFile, line);
        if(!removeLine){
            diretivesPipeline(inputFile, outputFile, line);
        }
        if (removeLine == false){
            outputFile << line << endl;
        }
    }

    printMNT();
    printMDT();
    printEquList();

    // Fechamento dos arquivos de input e output
    inputFile.close();
    outputFile.close();

    return;
}


// Função para gerar .txt com a MNT
void PreProcessing::printMNT() {
  ofstream file("MNT.txt");

  for (vector<MacroName *>::iterator it = MNT.begin(); it != MNT.end(); it++) {
    file << (*it)->getName() << " " << (*it)->getNumArgs() << " " << (*it)->getIndexMDT() << endl;
  }

  file.close();
}

// Função para gerar .txt com a MDT
void PreProcessing::printMDT() {
  ofstream file("MDT.txt");

  for (size_t i = 0; i < MDT.size(); i++) {
    file << "Linha " << i << ":" << endl;
    file << MDT[i]->getDefinition() << endl;
  }

  file.close();
}

// Função para gerar .txt com diretivas EQU
void PreProcessing::printEquList() {
    ofstream file("EQU.txt");

    for (auto equ : equList) {
        file << "Rótulo: " << equ->getLabel() << " | Valor: " << equ->getValue() << endl;
    }
}
