#include "../include/utils.hpp"


void findAndReplaceAll(string &data, string toSearch, string replaceStr){

    size_t pos = data.find(toSearch);

    while( pos != string::npos){
        data.replace(pos, toSearch.size(), replaceStr);
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}


string lineCleanComments(string line){

    size_t indice = line.find(";");

    if (indice != string::npos){
        line.erase(indice, line.size());
    }
    return line;
}


string lineCleanExtraBlanks(string line){

    line.erase(unique(line.begin(), line.end(), [] (char a, char b) { return isspace(a) && isspace(b); }), line.end());

    if (isspace(line.front())){
        line.erase(0, 1);
    }

    if (isspace(line.back())){
        line.pop_back();
    }

    return line;
}


string lineCleaning(string line){

    line = lineCleanComments(line);
    line = lineCleanExtraBlanks(line);

    return line;
}

string lineUpperCase(string line){

    transform(line.begin(), line.end(),line.begin(), ::toupper);

    return line;
}


void lineFormatting(ifstream &inputFile, string &line){

    string nextLine;

    line = lineCleaning(line);

    // Verifica se a linha restante é vazia ou apenas espaços em branco e descarta se verdadeiro
    while (line.empty() || all_of(line.begin(), line.end(), [](char c){return isspace(c);})) {
        getline(inputFile, line);
        line = lineCleaning(line);
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

            while (nextLine.empty() || all_of(nextLine.begin(), nextLine.end(), [](char c){return isspace(c);})) {
                getline(inputFile, nextLine);
                nextLine = lineCleaning(nextLine);
            }
            nextLine = lineUpperCase(nextLine);

            // Concatena linha com conteúdo ao rótulo no qual ocorreu quebra de linha
            line += " " + nextLine;
        }
    }
}