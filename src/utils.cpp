#include "../include/utils.hpp"

// Função para substituir todas as aparições de uma substring por outra
void findAndReplaceAll(string &str, string searching, string replacing){

    size_t indice = str.find(searching);

    while (indice != string::npos) {
        str.replace(indice, searching.size(), replacing);
        indice = str.find(searching, indice + replacing.size());
    }

}

// Função para remover comentários (iniciados pelo caracter ;) dentro de uma string
string lineCleanComments(string line){

    size_t indice = line.find(";");

    if (indice != string::npos){
        line.erase(indice, line.size());
    }
    return line;
}

// Função para substituir sequências de espaços em branco por somente um espaço, além de remover espaços em branco
// presentes nas extremidades da string
string lineCleanExtraBlanks(string line){
    // Remove sequência de espaços em branco desnecessária
    replace(line.begin(), line.end(), '\t', ' ');
    line.erase(unique(line.begin(), line.end(), [] (char a, char b) { return isspace(a) && isspace(b); }), line.end());

    // Remove espaços em branco das extremidades
    if (isspace(line.front())){
        line.erase(0, 1);
    }
    if (isspace(line.back())){
        line.pop_back();
    }

    return line;
}

// Função que realiza chamadas das funções para limpeza de espaços em branco e comentários de uma string
string lineCleaning(string line){

    line = lineCleanComments(line);
    line = lineCleanExtraBlanks(line);

    return line;
}

// Função que modifica todos os caracteres de uma string para caixa alta (Maiúscula)
string lineUpperCase(string line){

    transform(line.begin(), line.end(),line.begin(), ::toupper);

    return line;
}
