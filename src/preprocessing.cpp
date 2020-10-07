#include "../include/preprocessing.hpp"
#include "../include/utils.hpp"

bool removeLine, declaration, insideMacroDeclaration;


void PreProcessing::macroDeclaration(ifstream &inputFile, ofstream &outputFile, string &line){

    string label, token, newLine;
    string buffer = "";
    vector<string> argsList;

    replace(line.begin(), line.end(), ':', ' ');
    replace(line.begin(), line.end(), ',', ' ');

    line.erase(line.find(" MACRO"), 6);

    stringstream ss(line);

    insideMacroDeclaration = true;

    bool getLabel = false;
    while(ss >> token){
        if (!getLabel){
            label = token;
            getLabel = true;
        } else{
            argsList.push_back(token);
        }
    }

    MacroName *macroName = new MacroName();
    macroName->setName(label);
    macroName->setNumArgs(argsList.size());
    macroName->setIndexMDT(MDT.size());
    MNT.push_back(macroName);

    while(getline(inputFile, line)){

        lineFormatting(inputFile, line);

        if(line.find("ENDMACRO") != string::npos){
            buffer += line;
            break;
        }

        diretivesPipeline(inputFile, outputFile, line);

        if(removeLine){
            continue;
        }

        newLine = line;
        replace(newLine.begin(), newLine.end(), ':', ' ');
        replace(newLine.begin(), newLine.end(), ',', ' ');

        stringstream ss1(newLine);
        while(ss1 >> token){
            if (token.find('&') != string::npos){
                for (size_t i = 0; i < argsList.size(); i++){
                    if(token == argsList[i]){
                        line = line.replace(line.find(token), token.size(), to_string(i));
                    }
                }
            }
        }
        buffer += line + "\n";
    }

    MacroDefinition *macroDefinition = new MacroDefinition();
    macroDefinition->setDefinition(buffer);
    MDT.push_back(macroDefinition);

    insideMacroDeclaration = false;
}


void PreProcessing::checkMacro(ofstream &outputFile, string &line){

    string token;
    string newLine = line;
    string buffer;

    replace(newLine.begin(), newLine.end(), ':', ' ');
    replace(newLine.begin(), newLine.end(), ',', ' ');

    stringstream ss(newLine);

    while(ss >> token){
        for (vector<MacroName*>::iterator it = MNT.begin(); it != MNT.end(); it++) {
            if ((*it)->getName() == token) {
                buffer = MDT[(*it)->getIndexMDT()]->getDefinition();
                buffer.erase(buffer.find("ENDMACRO"), 8);

                for (int i = 0; i < (*it)->getNumArgs(); i++) {
                    ss >> token;
                    findAndReplaceAll(buffer, to_string(i), token);
                }

                // Remover \n do final do buffer
                buffer.pop_back();

                // Linha a ser escrita será o buffer que contém o corpo da macro
                line = buffer;
                break;
            }
        }
    }
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


string PreProcessing::checkEqu(string &line){

    string token, value;
    string newLine = line;

    replace(newLine.begin(), newLine.end(), ':', ' ');
    replace(newLine.begin(), newLine.end(), ',', ' ');

    stringstream ss(newLine);

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


void PreProcessing::ifClause(ifstream &inputFile, string &line) {

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
    // Descarta a próxima linha e retorna
    getline(inputFile, line);

    return;
}


void PreProcessing::diretivesPipeline(ifstream &inputFile, ofstream &outputFile, string &line){

    removeLine = false;
    declaration = false;

    if (line.find("MACRO") != string::npos){
        macroDeclaration(inputFile, outputFile, line);
        removeLine = true;
        declaration = true;
    }

    if (line.find("EQU") != string::npos){
        equDeclaration(line);
        removeLine = true;
        declaration = true;
    }

    if (line.find("IF") != string::npos){
        ifClause(inputFile, line);
        removeLine = true;
    }

    if (!declaration) {
        checkMacro(outputFile, line);
        checkEqu(line);
    }
    
}


void PreProcessing::runPreProcessing(string filename){

    ifstream inputFile(filename);
    ofstream outputFile(filename.substr(0, filename.find('.')) + ".pre");
    string line;

    while(getline(inputFile, line)){
        lineFormatting(inputFile, line);
        diretivesPipeline(inputFile, outputFile, line);
        if (removeLine == false && insideMacroDeclaration == false){
            outputFile << line << endl;
        }
    }

    printMNT();
    printMDT();
    printEquList();

    inputFile.close();
    outputFile.close();

    return;
}






//REMOVER
void PreProcessing::printMNT() {
  ofstream file("MNT.txt");

  for (vector<MacroName *>::iterator it = MNT.begin(); it != MNT.end(); it++) {
    file << (*it)->getName() << " " << (*it)->getNumArgs() << " " << (*it)->getIndexMDT() << endl;
  }

  file.close();
}

void PreProcessing::printMDT() {
  ofstream file("MDT.txt");

  for (size_t i = 0; i < MDT.size(); i++) {
    file << "Line " << i << ":" << endl;
    file << MDT[i]->getDefinition() << endl;
  }

  file.close();
}

void PreProcessing::printEquList() {
    ofstream file("EQU.txt");

    for (vector<EquDirective *>::iterator it = equList.begin(); it != equList.end(); it++) {
        file << "Label: " << (*it)->getLabel() << " | Value: " << (*it)->getValue() << endl;
    }
}
