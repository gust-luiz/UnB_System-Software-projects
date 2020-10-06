#include "../include/preprocessing.hpp"
#include "../include/macro.hpp"


string PreProcessing::lineCleanComments(string line){
    int indice = line.find(";");

    if (indice != -1){
        line.erase(indice, line.size());
    }
    return line;
}


string PreProcessing::lineCleanExtraBlanks(string line){

    replace(line.begin(), line.end(), '\t', ' ');
    line.erase(unique(line.begin(), line.end(), [] (char a, char b) { return a == ' ' && b == ' '; }), line.end());

    if (line.front() == ' '){
        line.erase(0, 1);
    }

    if (line.back() == ' '){
        line.pop_back();
    }

    return line;
}


string PreProcessing::lineCleaning(string line){

    line = lineCleanComments(line);
    line = lineCleanExtraBlanks(line);

    return line;
}

string PreProcessing::lineUpperCase(string line){

    transform(line.begin(), line.end(),line.begin(), ::toupper);

    return line;
}


void PreProcessing::equDeclaration(string line){

    string label, value;

    replace(line.begin(), line.end(), ':', ' ');
    line.erase(line.find(" EQU"), 4);

    stringstream ss(line);

    ss >> label >> value;

    EquDirective *equDirective = new EquDirective();
    equDirective->setLabel(label);
    equDirective->setValue(value);
    equList.push_back(equDirective);
}


string PreProcessing::checkEqu(string line){

    string token, value;
    string newLine = line;

    replace(newLine.begin(), newLine.end(), ':', ' ');
    replace(newLine.begin(), newLine.end(), ',', ' ');

    stringstream ss(newLine);

    while(ss >> token){
        for (vector<EquDirective*>::iterator it = equList.begin(); it != equList.end(); it++) {
            if((*it)->getLabel() == token){
                value = stoi((*it)->getValue());
                line = line.replace(line.find(token), token.size(), value);
                break;
            }
        }
    }

    return line;
}


void PreProcessing::ifClause(ifstream &inputFile, string line) {

    string ifExpression;
    int ifValue;

    line.erase(line.find("IF "), 3);

    stringstream ss(line);
    
    ss >> ifExpression;

    for (vector<EquDirective*>::iterator it = equList.begin(); it != equList.end(); it++) {
        if((*it)->getLabel() == ifExpression){
            ifValue = stoi((*it)->getValue());
            break;
        }
    }
    
    // Se expressão maior que zero retorna para permitir leitura da próxima linha
    if(ifValue > 0){
        return;
    }
    // Realiza leitura da próxima linha para descarte e retorna
    getline(inputFile, line);
    return;
}


void PreProcessing::printEquList() {
    for (vector<EquDirective *>::iterator it = equList.begin(); it != equList.end(); it++) {
        cout << "Label: " << (*it)->getLabel() << " | Value: " << (*it)->getValue() << endl;
    }
}


void PreProcessing::runPreProcessing(string filename){

    Macro *macro = new Macro();
    ifstream inputFile(filename);
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".pre");
    string line, nextLine;
    bool removeLine, declaration;
    
    while(getline(inputFile, line)){
        removeLine = false;
        declaration = false;

        line = lineCleaning(line);

        // Check if line is empty or with blank spaces and discard if true
        if (line.empty() || all_of(line.begin(), line.end(), [](char c){return isspace(c);})) {
            continue;
        }

        line = lineUpperCase(line);

        if (line.find(':') != -1){
            int pos = line.find(':');
            string aux = line.substr(pos+1);
            if (aux.empty() || (aux.compare(" ") == 0)){
                getline(inputFile, nextLine);
                nextLine = lineCleaning(nextLine);

                while (nextLine.empty() || all_of(nextLine.begin(), nextLine.end(), [](char c){return isspace(c);})) {
                    getline(inputFile, nextLine);
                    nextLine = lineCleaning(nextLine);
                }
                nextLine = lineUpperCase(nextLine);

                line += " " + nextLine;
            }

            if (line.find("MACRO") != -1){
                macro->macroDeclaration(inputFile, line);
                removeLine = true;
                declaration = true;
            }

            if (line.find("EQU") != -1){
                equDeclaration(line);
                removeLine = true;
                declaration = true;
            }


        }

        if (line.find("IF") != -1){
            ifClause(inputFile, line);
            removeLine = true;
        }

        if (!declaration) {
            macro->checkMacro(line, outputFile, removeLine);
            checkEqu(line);
        }

        if (removeLine == false){
            outputFile << line << endl;
        }

    }

    macro->printMNT();
    macro->printMDT();
    printEquList();

    inputFile.close();
    outputFile.close();

    return;
}
