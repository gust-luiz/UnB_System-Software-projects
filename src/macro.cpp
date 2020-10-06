#include "../include/macro.hpp"

void Macro::macroDeclaration(ifstream& inputFile, string line){

    replace(line.begin(), line.end(), ':', ' ');
    replace(line.begin(), line.end(), ',', ' ');
    line.erase(line.find(" MACRO"), 6);

    stringstream ss(line);
    string label, token;
    vector<string> argsList;

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

    string buffer = "";
    while(getline(inputFile, line)){
        string newLine;

        line = lineCleaning(line);
        line = lineUpperCase(line);

        if(line.find("ENDMACRO") != -1){
            buffer += line;
            break;
        }

        newLine = line;
        replace(newLine.begin(), newLine.end(), ':', ' ');
        replace(newLine.begin(), newLine.end(), ',', ' ');

        stringstream ss1(newLine);
        while(ss1 >> token){
            if (token.find('&') != -1){
                for (int i=0; i < argsList.size(); i++){
                    if(token == argsList[i]){
                        line = line.replace(line.find(token), token.size(), to_string(i));
                    }
                }
            }
        }
        buffer = buffer + line + "\n";
    }

    MacroDefinition *macroDefinition = new MacroDefinition();
    macroDefinition->setDefinition(buffer);
    MDT.push_back(macroDefinition);
}



//REMOVER
void Macro::printMNT() {
  ofstream file("MNT.txt");

  for (vector<MacroName *>::iterator it = MNT.begin(); it != MNT.end(); it++) {
    file << (*it)->getName() << " " << (*it)->getNumArgs() << " " << (*it)->getIndexMDT() << endl;
  }

  file.close();
}

void Macro::printMDT() {
  ofstream file("MDT.txt");

  for (int i = 0; i < MDT.size(); i++) {
    file << "Line " << i << ":" << endl;
    file << MDT[i]->getDefinition() << endl;
  }

  file.close();
}